
#include <assert.h>
#include <errno.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/xattr.h>


#include "tax.h"



void print_tax(tax *t)
{
    assert(t != NULL);

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


int rmtax(const char *filename)
{
    if (filename == NULL) {
        errno = EINVAL;
        return -1;
    }
    return removexattr(filename, "user.taxon");
}


