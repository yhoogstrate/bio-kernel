#include <stdlib.h>
#include <stdio.h>


void usage_bio()
{
	printf("Usage: bio [COMMAND]\n");
	printf("\n");
	printf("Commands:\n");
	printf("  ls          List information about the file with taxonomy info\n");
	printf("  chtax       Change taxonomy attribute\n");
	printf("  rmtax       Remove taxonomy attribute\n");
	printf("\n");
	printf("Run 'bio COMMAND --help' for more information on a command.\n");

}


int main(int argc, char *argv[])
{

	if(argc < 2)
	{
		usage_bio();
	}
	else
	{
		//
	}
	
	

	
	return 0;
}
