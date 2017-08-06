/* OSMP-Test Labor BS
#01 Verwendung von Funktionalitäten vor OSMP_Init oder 
    nach Aufruf von OSMP_FINALIZE
gcc -c -ansi -Wall -Wconversion -o xx test_01.c
R. Mentrup  5/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "settings.h"

int main(int argc, char *argv[])
{
    int size=-1, rank=-1;
    int *n = NULL;

    UNUSED(argc);
    UNUSED(argv);

    if (OSMP_Size(&size) == OSMP_SUCCESS) {
        printf("#01 OSMP_SIZE called before OSMP_Init\n");
        exit(EXIT_FAILURE);
    }


    if (OSMP_Init(&argc, &argv) != OSMP_SUCCESS) {
       printf("#00 OSMP_INIT Error\n");
       exit(EXIT_FAILURE);
    }

    if ((OSMP_Rank(&rank) != OSMP_SUCCESS) || (rank==-1)) {
        printf("#01 OSMP_RANK Error\n");
        exit(EXIT_FAILURE);
    }

    if (OSMP_Size(n) == OSMP_SUCCESS) {
        printf("#01 OSMP_SIZE Nullpointer Error\n");
        exit(EXIT_FAILURE);
    }

    if (OSMP_Rank(n) == OSMP_SUCCESS) {
        printf("#01 OSMP_RANK Nullpointer Error\n");
        exit(EXIT_FAILURE);
    }

    if(OSMP_Finalize()!=OSMP_SUCCESS){
        printf("#01 OSMP_FINALIZE Error");
        exit(EXIT_FAILURE);
    }

   /* Aufruf nach Finalize sollte nicht gehen */
   if (OSMP_Rank(&rank) == OSMP_SUCCESS) {
        printf("#01 OSMP_RANK NACH FINALIZE Error\n");
        exit(EXIT_FAILURE);
    }
   if (OSMP_Size(&size) == OSMP_SUCCESS) {
        printf("#01 OSMP_SIZE NACH FINALIZE Error\n");
        exit(EXIT_FAILURE);
    }

    printf("#01 ############################ SUCCESS\n");
    return EXIT_SUCCESS;
}
