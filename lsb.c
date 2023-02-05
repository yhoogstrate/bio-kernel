#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/dir.h>
#include <sys/xattr.h>
#include <errno.h>



extern int alphasort(const void*,const void*);


/**
touch xattr-test
setfattr -n user.test -v "hello" xattr-test
getfattr xattr-test
 */

// chattr
// lsattr .
// setfattr -n "taxon" -v 9606 data/test

int main(int argc, char *argv[])
{
	int i;
	i = setxattr("Makefile", "user.taxon", "9606", 256, 0);
	printf("i: %i -- errno = %i\n", i, errno);
	i = setxattr("data/test", "user.taxon", "9606", 256, 0);
	printf("i: %i -- errno = %i\n", i, errno);
	
	
	
	struct dirent **namelist;
	int n;
	
	if(argc < 1)
	{
		exit(EXIT_FAILURE);
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
		while(n--)
		{
			char taxon[256 + 1];
			taxon[0] = '\0';
			i = getxattr(namelist[n]->d_name, "user.taxon", taxon, 256);
			
			
			
			printf("%s\t%s\n",namelist[n]->d_name, taxon);
			free(namelist[n]);
		}
		
		free(namelist);
	}
	
	return 0;
}

