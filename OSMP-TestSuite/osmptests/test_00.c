/* OSMP-Test Labor BS
#00 Einfaches Testprogramm ohne besondere Fehlerquellen
     ->Nullpointer OSMP_Size
	 ->Nullpointer OSMP_Rank
gcc -c -ansi -Wall -Wconversion -o xx test_00.c
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
	
    if (OSMP_Init(&argc, &argv) != OSMP_SUCCESS) {
       printf("#00 OSMP_INIT Error\n");
       exit(EXIT_FAILURE);
    }

    if ((OSMP_Size(&size) != OSMP_SUCCESS) || (size!=1)) {
        printf("#00 OSMP_SIZE Error, Size was %d\n", size);
        exit(EXIT_FAILURE);
    }

    if ((OSMP_Rank(&rank) != OSMP_SUCCESS) || (rank==-1)) {
        printf("#00 OSMP_RANK Error\n");
        exit(EXIT_FAILURE);
    }

    if (OSMP_Size(n) == OSMP_SUCCESS) {
        printf("#00 OSMP_SIZE Nullpointer Error\n");
        exit(EXIT_FAILURE);
    }
	
    if (OSMP_Rank(n) == OSMP_SUCCESS) {
        printf("#00 OSMP_RANK Nullpointer Error\n");
        exit(EXIT_FAILURE);
    }

    if(OSMP_Finalize()!=OSMP_SUCCESS){
        printf("#00 OSMP_FINALIZE Error");
        exit(EXIT_FAILURE);
    }
	
    printf("#00 ############################ SUCCESS\n");
    return EXIT_SUCCESS;
}
