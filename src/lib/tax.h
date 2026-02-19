#ifndef TAX_H
#define TAX_H

#define SOCKET_PATH "/tmp/bio-kernel.sock"

typedef struct {
    unsigned int      taxon;           // taxon id
    long unsigned int file_pointer_db; // file offset for name string
} tax;

void        print_tax(tax *t);
int         chtax(const char *filename, const char *taxon);
int         rmtax(const char *filename);
const char *lookup_tax_name(const char *taxon_id);

#endif
