


#include <assert.h>
#include <errno.h>
#include <errno.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/xattr.h>
#include <unistd.h>




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
    
    return setxattr(filename, "user.taxon", taxon, 16, 0);// set to 16 ensures it sets the right bits afterwards
}


int rmtax(const char *filename)
{
    if (filename == NULL) {
        errno = EINVAL;
        return -1;
    }
    return removexattr(filename, "user.taxon");
}



// Returns a pointer to a static buffer — copy if you need to keep it.
// Returns NULL on failure.
const char *lookup_tax_name(const char *taxon_id)
{
    
    static char response[128];


    int fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (fd < 0)
    {
        return NULL;
    }


    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;

    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);


    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(fd);

        return NULL;
    }



    char request[64];
    snprintf(request, sizeof(request), "tax:%s:name\n", taxon_id);
    write(fd, request, strlen(request));



    ssize_t n = read(fd, response, sizeof(response) - 1);
    close(fd);


    if (n <= 0) return NULL;
    response[n] = '\0';
    response[strcspn(response, "\n")] = '\0';

    if (strncmp(response, "ERR:", 4) == 0) return NULL;



    return response;
}

