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


const char APPLICATION[] = "chgenome";

int chgenome(char* filename, char* taxon, char *reference)
{
    // @todo only overwrite with --force enabled
    int i;
    i = removexattr(filename, "user.taxon");
    i = setxattr(filename, "user.taxon", taxon, 16, 0);
    return i;
}


int main(int argc, char *argv[])
{

    if(argc < 2) {
        usage_help(APPLICATION);
    } else if(argc == 2) {
        if((strcmp(argv[1], "--help") == 0) | (strcmp(argv[1], "-h") == 0)) {
            usage_chgenome();
        } else {
            usage_help(APPLICATION);
        }
    } else {
        //validate tax
        //taxon tax = taxon(argv[1]);

        for(int i = 2; i < argc; i++) {
            chtax(argv[i], argv[1]);
        }
    }




    return 0;
}

