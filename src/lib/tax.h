

#define TAX_DB "ftp://ftp.ncbi.nlm.nih.gov/pub/taxonomy/taxdump.tar.gz";
#define SOCKET_PATH "/tmp/bio-kernel.sock"

typedef struct {
    unsigned int taxon; // taxon id
    long unsigned int file_pointer_db; // file location for reading string, preserves large amount of mem
} tax;


void print_tax(tax *);



int chtax(const char* filename, const char* tax);
int rmtax(const char* filename);
const char *lookup_tax_name(const char *taxon_id);
