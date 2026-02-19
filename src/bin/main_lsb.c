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
        int maxfilelen = 24;
        int spaces;

        for(int k = 0; k < n; k++) {
            spaces = maxfilelen;


            char taxon[256 + 1];
            taxon[0] = '\0';
            i = getxattr(namelist[k]->d_name, "user.taxon", taxon, 256);


            const int m = strlen(namelist[k]->d_name);
            
            /*
            if(strlen(taxon) > 0) {
                printf("%s  \e[0;32mHomo sapiens\033[0m    ", taxon);
                spaces -= 4 + 2 + 12 + 4;
            }
            */
            if (strlen(taxon) > 0) {
                const char *name = lookup_tax_name(taxon);
                if (name != NULL)
                {
                    #define TAXON_NAME_MAXLEN 22

                    char name_display[TAXON_NAME_MAXLEN + 1];
                    if (strlen(name) > TAXON_NAME_MAXLEN) {
                        strncpy(name_display, name, TAXON_NAME_MAXLEN - 2);
                        name_display[TAXON_NAME_MAXLEN - 2] = '\0';
                        strcat(name_display, "..");
                    } else {
                        strncpy(name_display, name, TAXON_NAME_MAXLEN);
                        name_display[TAXON_NAME_MAXLEN] = '\0';
                    }
                    printf("%s  \e[0;32m%s\033[0m  ", taxon, name_display);

                    spaces -= 4 + 2 + strlen(name) + 4;
                }
                else
                {
                    printf("%s  \e[0;31mUnknown\033[0m  ", taxon);
                    spaces -= 4 + 2 + 7 + 4;
                }
                
            }

            while(spaces > 0) {
                printf(" ");
                spaces--;
            }

            printf("%s", namelist[k]->d_name);

            free(namelist[k]);

            printf("\n");
        }

        free(namelist);
    }

    return 0;
}

