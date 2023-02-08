#include <stdlib.h>
#include <stdio.h>


#include "fuse.h"


int main(int argc, char *argv[])
{
	// mount to /bio/taxons/
	// mount to /bio/taxons-loaded/ [only loaded?]
	// make clever index of db/taxons
	
	fuse(argc, argv);
	
	return 0;
}

