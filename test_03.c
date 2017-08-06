/* OSMP-Test #03 Labor BS
#03  Test Blockierend send_recv
	->erwarte Blockade von Recv bis Send zum Ersten mal erfolgt
	->erwarte Blockade von Send nach 16 Prozessen 
R. Mentrup  5/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "OSMPLib.h"

#define MSGSIZE  32

void rand_str(char *dest, size_t length);

int main(int argc, char *argv[]) 
{
    int rank, source, len, size, target;
    char bufin[MSGSIZE], bufout[MSGSIZE];

    rank = source = len = size = -1;
	target = 1;
	
    if (OSMP_Init(&argc, &argv) != OSMP_SUCCESS) {
        printf("OSMP_INIT Error\n");
        exit(EXIT_FAILURE);
    }

    if (OSMP_Size(&size) != OSMP_SUCCESS || size != 43) {
        printf("OSMP_SIZE Error\n");
        exit(EXIT_FAILURE);
    }

    if (OSMP_Rank(&rank) != OSMP_SUCCESS || rank == -1) {
        printf("OSMP_RANK Error\n");
        exit(EXIT_FAILURE);
    }

	if (rank > 1) {
		printf("Waiting 5s before Sending\n");
		sleep(5);
		rand_str(bufout, MSGSIZE);
		printf("Process %d sending random message to process %d: %s\n", rank, target, bufout);
		if (OSMP_Send(bufout, MSGSIZE, target) != OSMP_SUCCESS) {
			printf("OSMP_SEND Error\n");
			exit(EXIT_FAILURE);
		}
	}
	else {
		printf("Process %d trying to receive msg (should block ~5 seconds)\n", rank);
		if (OSMP_Recv(bufin, MSGSIZE, &source, &len) != OSMP_SUCCESS) {
			printf("OSMP_RECV Error\n");
			exit(EXIT_FAILURE);
		}
		printf("Process %d received %d bytes from process %d: %s\n", rank, len, source, bufin);
	}

    if(OSMP_Finalize()!=OSMP_SUCCESS){
        printf("OSMP_FINALIZE Error");
        exit(EXIT_FAILURE);
    }
    
    printf("#03 Prozess %d ############################ SUCCESS\n", rank);
    return 0;
}

void rand_str(char *dest, size_t length) {
    char charset[] = "0123456789"
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 1) {
        int index = rand() % (int)(sizeof(charset) -1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}
