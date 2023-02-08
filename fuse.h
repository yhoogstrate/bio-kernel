
#ifndef FUSE_H
#define FUSE_H


typedef struct fuse_args { 
    int x;
} fuse_args;


int fuse(fuse_args *args);


#endif

