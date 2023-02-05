#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "utils.h"


void print_footer()
{
	printf("This software is free software\n");
	printf("Written and designed by Dr. Youri Hoogstrate\n");
	printf("Contact the author at: y.hoogstrate <at> gmail {dot} com\n");
	printf("bio-kernel online help: <https://github.com/yhoogstrate/bio-kernel>\n");
}


void usage_chtax()
{
	printf("Usage: chtax [OPTION]... taxon-id FILE...\n");
	printf("  or:  chtax -r/--remove FILE... taxons:\n");
	printf("  or:  chtax -f/--force FILE... taxons:\n");
	printf("Change the taxon of each FILE to taxon-id (number).\n");
	printf("\n");
	printf("Examples:\n");
	printf("  chtax 9606 hg19.fa\n");
	printf("  chtax 9606 alignment.bam alignment.bam.bai\n");
	printf("\n");
	printf("Removing taxons:\n");

	print_footer();
}

void usage_chtax_help()
{
	printf("chtax: missing operand\n");
	printf("Try 'chtax --help' for more information.\n");

	exit(EXIT_FAILURE);
}