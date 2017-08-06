/* OSMP-Test #08 Labor BS
#08 Senden mit NULL buffer (zize=2), anschließend einer TestRcv Aufruf
R. Mentrup  5/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "OSMPLib.h"


int testSendRecv(int rank);

int main(int argc, char *argv[]) {

    int rank = -1, size=-1;
	int ret, i;
	char *bufout;

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
	
    if (rank == 1) {
        bufout = NULL;
        printf("process %d trying to send NULL to process 1: %s\n", rank, bufout);
        for (i = 0; i < 16; i++) {
            if (OSMP_Send(bufout, 32, 2) == OSMP_SUCCESS) {
                printf("OSMP_SEND Error \n");
                exit(EXIT_FAILURE);
            }
        }
    }
	ret = testSendRecv(rank);
	
    if (OSMP_Finalize() != OSMP_SUCCESS) {
        printf("OSMP_FINALIZE Error");
        exit(EXIT_FAILURE);
    }
	
	if(rank==2){
		printf("#08 ############################ SUCCESS\n");
	}
	
    return ret;
}

int testSendRecv(int rank) {
    int *bufin, *bufout;
    int source, len;
    /*OSMP process 0*/
    if (rank == 1) {
        bufin = malloc(2 * sizeof(int));
        bufin[0] = 4711;
        bufin[1] = 4812;

        if (OSMP_Send(bufin, 2 * sizeof(int), 2) != OSMP_SUCCESS) {
            printf("osmp_SendRecv Error: OSMP_Send \n");
            free(bufin);
            return EXIT_FAILURE;
        }
        free(bufin);
        return EXIT_SUCCESS;
    } else if (rank == 2) {
        /*OSMP process 1*/
        bufout = malloc(2 * sizeof(int));

        if (OSMP_Recv(bufout, 2 * sizeof(int), &source, &len) != OSMP_SUCCESS) {
            printf("osmp_SendRecv Error: OSMP_Recv \n");

            return EXIT_FAILURE;
        }
        printf("OSMP process %d received %d byte from %d [%d:%d] \n", rank, len, source, bufout[0], bufout[1]);

        if (bufout[0] == 4711 && bufout[1] == 4812) {
            /*printf("Basic Send Recv is working fine\n");*/
            free(bufout);
            return EXIT_SUCCESS;
        }
    } else if (rank > 1) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
