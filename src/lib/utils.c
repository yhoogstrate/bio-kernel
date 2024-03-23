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

void usage_lsb()
{
    printf("Usage: lsb [OPTION]... FILE...\n");
    printf("List files with bio-kernel implemented annotations such as taxon of each FILE.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  lsb\n");
    printf("  lsb alignment.bam alignment.bam.bai\n");
    printf("  lsb -h alignment.bam alignment.bam.bai\n");
    printf("\n");
    print_footer();
}

void usage_chtax()
{
    printf("Usage: chtax [OPTION]... taxon-id [FILE]...\n");
    printf("  or:  chtax -r/--remove FILE... taxons:\n");
    printf("  or:  chtax -f/--force FILE... taxons:\n");
    printf("Change the taxon of each FILE to taxon-id (number).\n");
    printf("\n");
    printf("Examples:\n");
    printf("  chtax 9606 hg19.fa\n");
    printf("  chtax 9606 alignment.bam alignment.bam.bai\n");
    printf("\n");
    printf("Taxons can be removed using: rmtax\n");
    printf("\n");
    print_footer();
}

void usage_rmtax()
{
    printf("Usage: rmtax [FILE]...\n");
    printf("Remove the taxon of each FILE.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  rmtax alignment.bam alignment.bam.bai\n");
    printf("\n");
    printf("Taxons can be set using: chtax\n");
    printf("\n");

    print_footer();
}

void usage_chref()
{
    printf("Usage: chref [OPTION]... reference-build-id [FILE]...\n");
    printf("  or:  chref -r/--remove FILE... reference-build-id:\n");
    printf("  or:  chref -f/--force FILE... reference-build-id:\n");
    printf("Change the reference build of each FILE to reference-id (bio list ref).\n");
    printf("\n");
    printf("Examples:\n");
    printf("  chref hg19 hg19.fa\n");
    printf("  chref GRCh37.p1 alignment.bam alignment.bam.bai\n");
    printf("\n");
    printf("Taxons can be removed using: rmref\n");
    printf("\n");
    print_footer();
}


void usage_help(char *cmd)
{
    printf("%s: missing operand\n", cmd);
    printf("Try '%s --help' for more information.\n", cmd);

    exit(EXIT_FAILURE);
}


