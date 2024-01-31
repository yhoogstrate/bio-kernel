
#include <stdio.h>
#include <stdlib.h>

#include "tax.h"



void print_tax(tax *t)
{
    if(t == NULL)
    {
        exit(12);
    }
    
    printf("tax: %i\nfile pointer: %lu\n\n", t->taxon, t->file_pointer_db);
}

