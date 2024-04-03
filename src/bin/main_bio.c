#include <stdlib.h>
#include <stdio.h>

#include "config.h"


void usage_build_details()
{
    printf("bio v" BIOKERNEL_VERSION " -- part of the βio-Кernel project\n");
    printf("build: " COMPILE_DATE  "\n");
}


void usage_bio()
{
    printf("Usage: bio [COMMAND]\n");
    printf("\n");
    printf("Commands:\n");
    printf("  info           Small user guide\n");
    printf("  ls             List file/direftory information with bio/taxonomy data\n");
    printf("  list           List cached bio-kernel reference data\n");
    printf("  chtax          Change taxonomy attribute\n");
    printf("  rmtax          Remove taxonomy attribute\n");
    printf("  chref          Change reference sequence (e.g. hg19, GRCh37.p1, GRCh37.p2)\n");
    printf("  rmref          Change reference sequence (e.g. hg19, GRCh37.p1, GRCh37.p2)\n");
    printf("  stats          Obtain stats about cache [./config/uids.txt]\n");
    printf("  pull           Update a tax, ref or build\n");
    printf("  -E             Shows extended command list\n");
    printf("  -v, --version  Shows version and build info\n");
    printf("\n");
    printf("Extended command list:\n");
    printf("  chbuild        Change reference build (e.g. hg18, hg19, GRCh37)\n");
    printf("  rmbuild        Change reference build (e.g. hg18, hg19, GRCh37)\n");
    printf("\n");
    printf("Run 'bio COMMAND --help' for more information on a command.\n");

}


int main(int argc, char *argv[])
{

    if(argc < 2) {
        usage_bio();
    } else {
        usage_build_details();
        printf("\n");
        printf("Copyright (C) 2019 Dr. Youri Hoogstrate\n");
        printf("License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>.\n");
        printf("This is free software: you are free to change and redistribute it.\n");
        printf("There is NO WARRANTY, to the extent permitted by law.\n");
        printf("\n");
        printf("The βio-Кernel project is designed and written by Dr. Youri Hoogstrate.\n");
        printf("https://github.com/yhoogstrate/bio-kernel/\n");
    }




    return 0;
}
