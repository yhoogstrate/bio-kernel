#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/dir.h>
#include <sys/xattr.h>
#include <errno.h>


#include "utils.h"


extern int alphasort(const void*,const void*);




int chtax(char* filename, char* taxon) 
{
	// @todo only overwrite with --force enabled
	int i;
	i = removexattr( filename, "user.taxon");
	i = setxattr( filename, "user.taxon", taxon, 16, 0);
	return i;
}


int main(int argc, char *argv[])
{

	if(argc < 2)
	{
		usage_chtax_help();
	}
	else if(argc == 2)
	{
		if((strcmp(argv[1], "--help") == 0) | (strcmp(argv[1], "-h") == 0))
		{
			usage_chtax();
		}
		else
		{
			usage_chtax_help();
		}
	}
	else
	{
		//validate tax
		//taxon tax = taxon(argv[1]);
		
		for(int i = 2; i < argc; i++){
			chtax(argv[i], argv[1]);
		}
	}
	
	

	
	return 0;
}

