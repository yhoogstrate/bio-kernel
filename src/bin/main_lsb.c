#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/dir.h>
#include <sys/xattr.h>
#include <errno.h>


#include "src/lib/utils.h"
#include "src/lib/tax.h"


extern int alphasort(const void*, const void*);



int main(int argc, char *argv[])
{
    int i;



    struct dirent **namelist;
    int n;

    if(argc < 1) {
        exit(EXIT_FAILURE);
    } else if(argc == 2 && (strcmp(argv[1], "--help") == 0) | (strcmp(argv[1], "-h") == 0)) {
        usage_lsb();
    } else if(argc == 1) {
        n = scandir(".", &namelist, NULL, alphasort);
    } else {
        n = scandir(argv[1], &namelist, NULL, alphasort);
    }
    
    

    if(n < 0) {
        perror("scandir");
        exit(EXIT_FAILURE);
    } else {
        #define TAXON_NAME_MAXLEN 18

        // First pass: find max taxon length for column width (minimum 4)
        int max_taxon_len = 4;
        for (int k = 0; k < n; k++) {
            char taxon[256 + 1];
            taxon[0] = '\0';
            getxattr(namelist[k]->d_name, "user.taxon", taxon, 256);
            int tlen = strlen(taxon);
            if (tlen > max_taxon_len) max_taxon_len = tlen;
        }

        // Second pass: print
        for (int k = 0; k < n; k++) {
            char taxon[256 + 1];
            taxon[0] = '\0';
            i = getxattr(namelist[k]->d_name, "user.taxon", taxon, 256);

            if (strlen(taxon) > 0) {
                // Print taxon, padded to column width
                printf("%-*s  ", max_taxon_len, taxon);

                const char *name = lookup_tax_name(taxon);
                if (name != NULL) {
                    char name_display[TAXON_NAME_MAXLEN + 1];
                    if (strlen(name) > TAXON_NAME_MAXLEN) {
                        strncpy(name_display, name, TAXON_NAME_MAXLEN - 2);
                        name_display[TAXON_NAME_MAXLEN - 2] = '\0';
                        strcat(name_display, "..");
                    } else {
                        strncpy(name_display, name, TAXON_NAME_MAXLEN);
                        name_display[TAXON_NAME_MAXLEN] = '\0';
                    }
                    printf("\e[0;32m%-*s\033[0m  ", TAXON_NAME_MAXLEN, name_display);
                } else {
                    printf("\e[0;31m%-*s\033[0m  ", TAXON_NAME_MAXLEN, "Unknown");
                }
            } else {
                // No taxon: print blank columns
                printf("%-*s  %-*s  ", max_taxon_len, "", TAXON_NAME_MAXLEN, "");
            }

            printf("%s\n", namelist[k]->d_name);
            free(namelist[k]);
        }
        free(namelist);
    }

    return 0;
}

