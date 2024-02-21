/*
 * ref is s.t.h. like 'hg19' and 'hg38' a.k.a. build
 * 
 * 
 */


struct ref {
    char *uid;
    
    tax *tax;

    ref *previous_ref;
    ref *next_ref;
};

