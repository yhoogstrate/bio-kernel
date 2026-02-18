#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <search.h>
#include "config.h"
#include "src/lib/tax.h"

static int tax_compare(const void *a, const void *b)
{
    const tax *ta = (const tax *)a;
    const tax *tb = (const tax *)b;
    if (ta->taxon < tb->taxon) return -1;
    if (ta->taxon > tb->taxon) return  1;
    return 0;
}

int main(int argc, char *argv[])
{
    FILE *f = fopen("db/taxons/taxons", "r");
    if (f == NULL) {
        fprintf(stderr, "error: could not open db/taxons/taxons\n");
        return 1;
    }

    printf("loading file: db/taxons/taxons\n");

    void *root = NULL;
    char line[256];

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#') continue;

        unsigned int id;
        if (sscanf(line, "%u", &id) != 1) continue;

        char *tab = strchr(line, '\t');
        if (tab == NULL) continue;

        long offset = ftell(f) - (long)strlen(tab) + 1;

        tax *entry = malloc(sizeof(tax));
        if (entry == NULL) {
            fprintf(stderr, "error: malloc failed\n");
            fclose(f);
            return 1;
        }
        entry->taxon = id;
        entry->file_pointer_db = offset;

        tsearch(entry, &root, tax_compare);
    }

    // lookup
    unsigned int key_id = 7;
    tax key = { .taxon = key_id, .file_pointer_db = 0 };
    void **result = tfind(&key, &root, tax_compare);

    if (result != NULL) {
        tax *found = *(tax **)result;
        printf("found taxon %u at file offset %lu\n", found->taxon, found->file_pointer_db);
        fseek(f, found->file_pointer_db, SEEK_SET);
        char name[256];
        if (fgets(name, sizeof(name), f) != NULL) {
            name[strcspn(name, "\n")] = '\0';
            printf("name: %s\n", name);
        }
    } else {
        printf("taxon %u not found\n", key_id);
    }

    fclose(f);
    return 0;
}

