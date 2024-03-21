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

    if(argc < 2) {
        usage_chtax_help();
    } else if(argc == 2) {
        if((strcmp(argv[1], "--help") == 0) | (strcmp(argv[1], "-h") == 0)) {
            usage_chtax();
        } else {
            usage_chtax_help();
        }
    } else {
        tax t;
        t.taxon = 9606;
        t.file_pointer_db = 456;

        for(int i = 2; i < argc; i++) {
            chtax(argv[i], argv[1]);
        }
    }




    return 0;
}

