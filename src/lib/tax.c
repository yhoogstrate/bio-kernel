
#include <stdio.h>
#include <stdlib.h>

#include "tax.h"



void print_tax(tax *t)
{
    if(t == NULL) {
        exit(12);
    }

    printf("tax: %i\nfile pointer: %lu\n\n", t->taxon, t->file_pointer_db);
}




int chtax(char* filename, char* taxon)
{
    // @todo only overwrite with --force enabled
    int i;
    i = removexattr(filename, "user.taxon");
    i = setxattr(filename, "user.taxon", taxon, 16, 0);
    return i;

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
