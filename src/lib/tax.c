

#include <errno.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/xattr.h>


#include "tax.h"



void print_tax(tax *t)
{
    if(t == NULL) {
        exit(12);
    }

    printf("tax: %i\nfile pointer: %lu\n\n", t->taxon, t->file_pointer_db);
}



int chtax(const char *filename, const char *taxon)
{
    if (filename == NULL || taxon == NULL) {
        errno = EINVAL;
        return -1;
    }
    removexattr(filename, "user.taxon");
    
    return setxattr(filename, "user.taxon", taxon, strlen(taxon), 0);
}


int rmtax(char* filename)
{
    // @todo only overwrite with --force enabled
    if(filename == NULL) {
        return -1;
    }
    
    int result;
    
    result = removexattr(filename, "user.taxon");
    
    return result;
}
