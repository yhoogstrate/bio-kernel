#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "tax.h"


size_t tax_read_ntax()
{
    FILE *fptr;
    fptr = fopen("../db/taxons/taxons", "r");

    char buffer[1024 + 1];
    fgets(buffer, 1024, fptr);
    fgets(buffer, 1024, fptr);
    char *start = strstr(buffer, "# n: ");
    char *end = strstr(buffer, "\n");
    fclose(fptr);

    size_t out = 0;

    if(start != NULL && end != NULL) {
        char *int_char = start + 5;
        *(end) = '\0';

        out = (size_t) atoi(int_char);
    }



    return out;
}


int main(int argc, char *argv[])
{
    printf("loading file: ../db/taxons/taxons\n");
    size_t ntax = tax_read_ntax();
    printf("n: %i\n--\n", (unsigned int) ntax);


    tax **taxon_list = malloc((ntax + 1) * sizeof(tax*));


    FILE *fptr;
    fptr = fopen("../db/taxons/taxons", "r");
    char buffer[1024 + 1];

    ntax = 5;
    size_t i = 0;
    size_t fp = 0;
    while(fgets(buffer, 1024, fptr) && i < ntax) {
        if(buffer[0] != '#') {
            size_t linelen = strlen(buffer);
            char *end1 = strstr(buffer, "\t");
            char *end2 = strstr(buffer, "\n");
            *(end1) = '\0';
            *(end2) = '\0';

            printf("{%s} ", buffer);
            printf("{%s} ", &end1[1]);
            printf("{%lu} ", fp + (&end1[1] - &buffer[0]) + 1);

            printf("\n");

            tax *t = (tax *) malloc(sizeof(tax));
            t->taxon = atoi(buffer);
            t->file_pointer_db = fp + (&end1[1] - &buffer[0]) + 1;

            taxon_list[i++] = t;

            print_tax(t);

            fp += linelen;
        } else {
            fp += strlen(buffer);
        }
    }


    fclose(fptr);





    return 0;
}
