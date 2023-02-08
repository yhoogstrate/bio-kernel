#include <stdio.h>


#include "fuse.h"


int fuse(fuse_args *args)
{
    if(args == NULL)
    {
        printf("err\n");
    }
    
    printf("interactive env mounting to /bio\n");
    
    return 0;
}

