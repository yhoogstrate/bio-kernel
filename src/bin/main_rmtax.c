#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/dir.h>
#include <sys/xattr.h>
#include <errno.h>


#include "src/lib/utils.h"


extern int alphasort(const void*, const void*);




int rmtax(char* filename)
{
    // @todo only overwrite with --force enabled
    int i;
    i = removexattr(filename, "user.taxon");
    return i;
}


int main(int argc, char *argv[])
{

    if(argc < 2) {
        usage_rmtax_help();
    } else if(argc == 2 & (strcmp(argv[1], "--help") == 0) | (strcmp(argv[1], "-h") == 0)) {
        usage_rmtax();
    } else {
        for(int i = 1; i < argc; i++) {
            rmtax(argv[1]);
        }
    }




    return 0;
}

