/* OSMP-Test #10 Labor BS
#10 Einfaches SendRecv (zize=2)
R. Mentrup  5/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "settings.h"


int main(int argc, char *argv[]) {
    int *bufin, *bufout;
    int source, len;
    int rank = -1, size=-1;

    if (OSMP_Init(&argc, &argv) != OSMP_SUCCESS) {
        printf("OSMP_INIT Error \n");
        exit(EXIT_FAILURE);
    }

    if (OSMP_Size(&size) != OSMP_SUCCESS || size!=2) {
        printf("OSMP_SIZE Error\n");
        exit(EXIT_FAILURE);
    }

    if (OSMP_Rank(&rank) != OSMP_SUCCESS || rank == -1) {
        printf("OSMP_RANK Error \n");
        exit(EXIT_FAILURE);
    }

    /*OSMP process 0*/
    if (rank == 0) {
        bufin = malloc(2 * sizeof(int));
        bufin[0] = 4711;
        bufin[1] = 4812;

        if (OSMP_Send(bufin, 2 * sizeof(int), 1) != OSMP_SUCCESS) {
            printf("osmp_SendRecv Error: OSMP_Send \n");
            free(bufin);
            return EXIT_FAILURE;
        }
        free(bufin);
        return EXIT_SUCCESS;
    } else if (rank == 1) {

        /*OSMP process 1*/
        bufout = malloc(2 * sizeof(int));

        if (OSMP_Recv(bufout, 2 * sizeof(int), &source, &len) != OSMP_SUCCESS) {
            printf("osmp_SendRecv Error: OSMP_Recv \n");

            return EXIT_FAILURE;
        }
        printf("OSMP process %d received %d byte from %d [%d:%d] \n", rank, len, source, bufout[0], bufout[1]);

        if (bufout[0] == 4711 && bufout[1] == 4812) {
            printf("#10 ############################ SUCCESS\n");
            free(bufout);
            return EXIT_SUCCESS;
        }
    } else if (rank > 1) {
        return EXIT_SUCCESS;
    }
	printf("Somthing went wrong\n");
    return EXIT_FAILURE;
}

