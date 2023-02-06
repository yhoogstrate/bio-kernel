#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/dir.h>
#include <sys/xattr.h>
#include <errno.h>


#include "utils.h"


extern int alphasort(const void*,const void*);



int main(int argc, char *argv[])
{
	int i;

	
	
	struct dirent **namelist;
	int n;
	
	if(argc < 1)
	{
		exit(EXIT_FAILURE);
	}
	else if(argc == 2 && (strcmp(argv[1], "--help") == 0) | (strcmp(argv[1], "-h") == 0))
	{
		usage_lsb();
	}
	else if(argc == 1)
	{
		n = scandir(".", &namelist, NULL, alphasort);
	}
	else
	{
		n = scandir(argv[1], &namelist, NULL, alphasort);
	}
	
	
	if(n < 0)
	{
		perror("scandir");
		exit(EXIT_FAILURE);
	}
	else
	{
		int maxfilelen = 20;
		
		for(int k =0; k < n; k++)
		{
			char taxon[256 + 1];
			taxon[0] = '\0';
			i = getxattr(namelist[k]->d_name, "user.taxon", taxon, 256);
			
			
			printf("%s", namelist[k]->d_name);
			for(int l = maxfilelen - strlen(namelist[k]->d_name); l > 0; l--)
			{
				printf(" ");
			}
			printf("%s\n",taxon);
			free(namelist[k]);
		}
		
		free(namelist);
	}
	
	return 0;
}

