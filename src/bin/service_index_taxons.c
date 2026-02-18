#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "../lib/tax.h"


// ---------------------------------------------------------------
// Constants
// ---------------------------------------------------------------

#define CACHE_SIZE    256
#define NAME_MAX_LEN  128
#define TAXONS_FILE   "db/taxons/taxons"
#define INDEX_FILE    "db/taxons/taxons.idx"
#define BUF_SIZE    256
#define BACKLOG     8


// ---------------------------------------------------------------
// Disk index
// File layout:
//   [4 bytes: max_taxon_id][max_taxon_id * sizeof(off_t): offsets]
// Indexed directly by taxon ID, so lookup is O(1):
//   seek to sizeof(unsigned int) + id * sizeof(off_t), read one off_t.
// A value of 0 means the ID is not present.
// ---------------------------------------------------------------

// Pass 1: scan the file to find the highest taxon ID present.
static unsigned int tax_read_max_id(FILE *f)
{
    unsigned int max_id = 0;
    char line[512];
    rewind(f);
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#') continue;
        unsigned int id;
        if (sscanf(line, "%u", &id) == 1 && id > max_id)
            max_id = id;
    }
    return max_id;
}

// Build the flat binary index from the taxons source file.
// Pass 1 finds max_taxon_id, pass 2 populates offsets.
// Writes header + entries to index_path.
int tax_build_index(const char *taxons_path, const char *index_path)
{
    FILE *in = fopen(taxons_path, "r");
    if (in == NULL) {
        fprintf(stderr, "tax_build_index: could not open %s: %s\n",
                taxons_path, strerror(errno));
        return -1;
    }

    // Pass 1: determine max taxon ID
    unsigned int max_id = tax_read_max_id(in);
    if (max_id == 0) {
        fprintf(stderr, "tax_build_index: no taxon IDs found\n");
        fclose(in);
        return -1;
    }
    printf("tax_build_index: max taxon ID = %u\n", max_id);

    // Allocate index zeroed out (0 = not present)
    size_t n_entries = (size_t)max_id + 1;
    off_t *index = calloc(n_entries, sizeof(off_t));
    if (index == NULL) {
        fprintf(stderr, "tax_build_index: calloc failed\n");
        fclose(in);
        return -1;
    }

    // Pass 2: populate offsets
    rewind(in);
    char line[512];
    off_t line_start = 0;

    while (fgets(line, sizeof(line), in)) {
        if (line[0] == '#') {
            line_start += (off_t)strlen(line);
            continue;
        }

        unsigned int id;
        if (sscanf(line, "%u", &id) != 1) {
            line_start += (off_t)strlen(line);
            continue;
        }

        char *tab = strchr(line, '\t');
        if (tab == NULL) {
            line_start += (off_t)strlen(line);
            continue;
        }

        // offset points to the first character of the name
        index[id] = line_start + (off_t)(tab - line) + 1;
        line_start += (off_t)strlen(line);
    }

    fclose(in);

    // Write index file: header (max_id) followed by entries
    FILE *out = fopen(index_path, "wb");
    if (out == NULL) {
        fprintf(stderr, "tax_build_index: could not open %s for writing: %s\n",
                index_path, strerror(errno));
        free(index);
        return -1;
    }

    if (fwrite(&max_id, sizeof(unsigned int), 1, out) != 1) {
        fprintf(stderr, "tax_build_index: failed to write header\n");
        fclose(out);
        free(index);
        return -1;
    }

    size_t written = fwrite(index, sizeof(off_t), n_entries, out);
    fclose(out);
    free(index);

    if (written != n_entries) {
        fprintf(stderr, "tax_build_index: short write\n");
        return -1;
    }

    printf("tax_build_index: wrote %zu entries to %s\n", written, index_path);
    return 0;
}

// Look up the file offset for a given taxon ID from the disk index.
// Returns -1 if not found or on error.
off_t tax_index_lookup(FILE *index_file, unsigned int id)
{
    // Read max_id from header
    rewind(index_file);
    unsigned int max_id;
    if (fread(&max_id, sizeof(unsigned int), 1, index_file) != 1) return -1;
    if (id > max_id) return -1;

    // Entries start after the header
    long entry_offset = (long)sizeof(unsigned int) + (long)(id * sizeof(off_t));
    if (fseek(index_file, entry_offset, SEEK_SET) != 0) return -1;

    off_t offset;
    if (fread(&offset, sizeof(off_t), 1, index_file) != 1) return -1;
    if (offset == 0) return -1;

    return offset;
}

// ---------------------------------------------------------------
// LRU cache (256 entries, in memory)
// Stores taxon id + name for recently looked up taxa.
// Evicts the least recently used entry on a miss.
// ---------------------------------------------------------------

typedef struct {
    unsigned int taxon;
    char name[NAME_MAX_LEN];
    unsigned long last_used;  // logical clock, higher = more recent
    int valid;                // 0 = empty slot
} tax_cache_entry;

typedef struct {
    tax_cache_entry entries[CACHE_SIZE];
    unsigned long clock;
    FILE *taxons_file;
    FILE *index_file;
} tax_cache;

tax_cache *tax_cache_open(const char *taxons_path, const char *index_path)
{
    tax_cache *c = calloc(1, sizeof(tax_cache));
    if (c == NULL) return NULL;

    c->taxons_file = fopen(taxons_path, "r");
    if (c->taxons_file == NULL) {
        fprintf(stderr, "tax_cache_open: could not open %s: %s\n",
                taxons_path, strerror(errno));
        free(c);
        return NULL;
    }

    c->index_file = fopen(index_path, "rb");
    if (c->index_file == NULL) {
        fprintf(stderr, "tax_cache_open: could not open %s: %s\n",
                index_path, strerror(errno));
        fclose(c->taxons_file);
        free(c);
        return NULL;
    }

    return c;
}

void tax_cache_close(tax_cache *c)
{
    if (c == NULL) return;
    fclose(c->taxons_file);
    fclose(c->index_file);
    free(c);
}

// Returns a pointer to the cached name string, or NULL if not found.
const char *tax_cache_lookup(tax_cache *c, unsigned int id)
{
    // 1. Search cache
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (c->entries[i].valid && c->entries[i].taxon == id) {
            c->entries[i].last_used = ++c->clock;
            return c->entries[i].name;
        }
    }

    // 2. Cache miss — look up offset in disk index
    off_t offset = tax_index_lookup(c->index_file, id);
    if (offset < 0) return NULL;

    // 3. Read name from taxons file
    if (fseek(c->taxons_file, offset, SEEK_SET) != 0) return NULL;
    char buf[NAME_MAX_LEN];
    if (fgets(buf, sizeof(buf), c->taxons_file) == NULL) return NULL;
    buf[strcspn(buf, "\n")] = '\0';

    // 4. Find LRU slot to evict
    int lru = 0;
    for (int i = 1; i < CACHE_SIZE; i++) {
        if (!c->entries[i].valid) { lru = i; break; }
        if (c->entries[i].last_used < c->entries[lru].last_used) lru = i;
    }

    // 5. Insert into cache
    c->entries[lru].taxon = id;
    strncpy(c->entries[lru].name, buf, NAME_MAX_LEN - 1);
    c->entries[lru].name[NAME_MAX_LEN - 1] = '\0';
    c->entries[lru].last_used = ++c->clock;
    c->entries[lru].valid = 1;

    return c->entries[lru].name;
}

// Dispatch a parsed request and write the response to client_fd.
// Protocol: "resource:id:field\n"
// Response: null-terminated string, or "ERR:...\n" on failure.
static void handle_request(int client_fd, tax_cache *cache, char *buf)
{
    char resource[32], field[32];
    unsigned int id;

    // Strip trailing newline
    buf[strcspn(buf, "\n")] = '\0';

    if (sscanf(buf, "%31[^:]:%u:%31[^:]", resource, &id, field) != 3) {
        write(client_fd, "ERR:invalid request\n", 20);
        return;
    }

    if (strcmp(resource, "tax") == 0) {
        if (strcmp(field, "name") == 0) {
            const char *name = tax_cache_lookup(cache, id);
            if (name == NULL || strlen(name) == 0) {
                write(client_fd, "ERR:not found\n", 14);
            } else {
                printf("response: %s\n", name);
                write(client_fd, name, strlen(name));
                write(client_fd, "\n", 1);
            }
        } else {
            write(client_fd, "ERR:unknown field\n", 18);
        }
    } else {
        write(client_fd, "ERR:unknown resource\n", 21);
    }
}


// access by: echo "tax:9606:name" | socat - UNIX-CONNECT:/tmp/bio-kernel.sock
int main(void)
{
    // Step 1: build the disk index (two-pass: find max_id, then populate)
    printf("=== building index ===\n");
    if (tax_build_index(TAXONS_FILE, INDEX_FILE) != 0) {
        fprintf(stderr, "failed to build index\n");
        return 1;
    }

    // Step 2: open cache backed by disk index
    printf("\n=== opening cache ===\n");
    tax_cache *cache = tax_cache_open(TAXONS_FILE, INDEX_FILE);
    if (cache == NULL) {
        fprintf(stderr, "failed to open cache\n");
        return 1;
    }

    // Step 3: test lookups via cache
    printf("\n=== lookups ===\n");
    unsigned int test_ids[] = { 1, 2, 6, 7, 9, 10, 11, 13, 99999, 7 };
    size_t n = sizeof(test_ids) / sizeof(test_ids[0]);

    for (size_t i = 0; i < n; i++) {
        unsigned int id = test_ids[i];
        const char *name = tax_cache_lookup(cache, id);
        if (name != NULL)
            printf("taxon %6u => %s\n", id, name);
        else
            printf("taxon %6u => not found\n", id);
    }

    // Step 4: show that second lookup for id=7 is a cache hit (no disk seek)
    printf("\n=== cache hit test (taxon 7 again) ===\n");
    const char *name = tax_cache_lookup(cache, 7);
    printf("taxon 7 => %s\n", name ? name : "not found");

    tax_cache_close(cache);

    // Step 5: raw disk seek — bypass cache entirely, go straight to index
    // and read the name directly from the taxons file
    printf("\n=== raw disk seek (no cache) ===\n");
    FILE *index_file  = fopen(INDEX_FILE,  "rb");
    FILE *taxons_file = fopen(TAXONS_FILE, "r");

    if (index_file == NULL || taxons_file == NULL) {
        fprintf(stderr, "step 5: could not open files\n");
        return 1;
    }

    unsigned int raw_ids[] = { 2, 9, 13 };
    size_t raw_n = sizeof(raw_ids) / sizeof(raw_ids[0]);

    for (size_t i = 0; i < raw_n; i++) {
        unsigned int id = raw_ids[i];

        off_t offset = tax_index_lookup(index_file, id);
        if (offset < 0) {
            printf("taxon %6u => not found in index\n", id);
            continue;
        }

        printf("taxon %6u => index offset: %ld => ", id, (long)offset);

        if (fseek(taxons_file, offset, SEEK_SET) != 0) {
            printf("fseek failed\n");
            continue;
        }

        char raw[NAME_MAX_LEN];
        if (fgets(raw, sizeof(raw), taxons_file) == NULL) {
            printf("fgets failed\n");
            continue;
        }
        raw[strcspn(raw, "\n")] = '\0';
        printf("%s\n", raw);
    }

    fclose(index_file);
    fclose(taxons_file);
    
    
    /// now start a cache service
    
    // Open cache
    tax_cache *cache2 = tax_cache_open(TAXONS_FILE, INDEX_FILE);
    if (cache2 == NULL) {
        fprintf(stderr, "taxd: failed to open cache\n");
        return 1;
    }
    printf("taxd: cache open\n");

    // Create Unix domain socket
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        fprintf(stderr, "taxd: socket: %s\n", strerror(errno));
        tax_cache_close(cache2);
        return 1;
    }

    // Remove stale socket file if it exists
    unlink(SOCKET_PATH);

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "taxd: bind: %s\n", strerror(errno));
        close(server_fd);
        tax_cache_close(cache2);
        return 1;
    }

    if (listen(server_fd, BACKLOG) < 0) {
        fprintf(stderr, "taxd: listen: %s\n", strerror(errno));
        close(server_fd);
        tax_cache_close(cache2);
        return 1;
    }

    printf("taxd: listening on %s\n", SOCKET_PATH);

    // Main accept loop
    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) {
            fprintf(stderr, "taxd: accept: %s\n", strerror(errno));
            continue;
        }

        char buf[BUF_SIZE];
        ssize_t n = read(client_fd, buf, sizeof(buf) - 1);
        if (n <= 0) {
            close(client_fd);
            continue;
        }
        buf[n] = '\0';

        printf("taxd: request: %s\n", buf);
        handle_request(client_fd, cache2, buf);

        close(client_fd);
    }

    close(server_fd);
    unlink(SOCKET_PATH);
    tax_cache_close(cache2);
    return 0;
}

