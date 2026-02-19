#define BIOKERNEL_TEST_BUILD
#include "../src/bin/bio-kerneld.c"


int main(void)
{
    printf("=== building index ===\n");
    if (tax_build_index(TAXONS_FILE, INDEX_FILE) != 0) {
        fprintf(stderr, "test_taxons: failed to build index\n");
        return 1;
    }

    printf("\n=== cache lookups ===\n");
    tax_cache *cache = tax_cache_open(TAXONS_FILE, INDEX_FILE);
    if (cache == NULL) {
        fprintf(stderr, "test_taxons: failed to open cache\n");
        return 1;
    }

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

    printf("\n=== cache hit test (taxon 7 again) ===\n");
    const char *name = tax_cache_lookup(cache, 7);
    printf("taxon 7 => %s\n", name ? name : "not found");

    tax_cache_close(cache);

    printf("\n=== raw disk seek (no cache) ===\n");
    FILE *index_file  = fopen(INDEX_FILE,  "rb");
    FILE *taxons_file = fopen(TAXONS_FILE, "r");
    if (index_file == NULL || taxons_file == NULL) {
        fprintf(stderr, "test_taxons: could not open files\n");
        if (index_file)  fclose(index_file);
        if (taxons_file) fclose(taxons_file);
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
    return 0;
}
