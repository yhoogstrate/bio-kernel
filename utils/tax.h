
#define TAX_DB "ftp://ftp.ncbi.nlm.nih.gov/pub/taxonomy/taxdump.tar.gz";



typedef struct {
    unsigned int taxon;
    unsigned int file_pointer_db;
} tax;


void print_tax(tax *);
