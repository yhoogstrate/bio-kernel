
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/dir.h>
#include <sys/xattr.h>
#include <errno.h>


#include "src/lib/utils.h"
#include "src/lib/db.h"


extern int alphasort(const void*, const void*);


const char APPLICATION[] = "chref";

int main(int argc, char *argv[])
{
	
	
	printf("[%i]\n", t9606.taxon);


    if(argc < 2) {
        usage_help(APPLICATION);
    } else if(argc == 2) {
        if((strcmp(argv[1], "--help") == 0) | (strcmp(argv[1], "-h") == 0)) {
            usage_chref();
        } else {
			usage_help(APPLICATION);
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

