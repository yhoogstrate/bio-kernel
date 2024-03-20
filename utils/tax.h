
#define TAX_DB "ftp://ftp.ncbi.nlm.nih.gov/pub/taxonomy/taxdump.tar.gz";



typedef struct {
    unsigned int taxon; // taxon id
    long unsigned int file_pointer_db; // file location for reading string, preserves large amount of mem
} tax;


void print_tax(tax *);



int chtax(char* filename, char* tax);
