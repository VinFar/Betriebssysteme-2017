/* OSMP-Test Labor BS
#04 Sender sendet groessere Nachrichten als Empfänger erwartet
gcc -c -ansi -Wall -Wconversion -o xx test_04.c
R. Mentrup  5/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "settings.h"

void rand_str(char *dest, size_t length);

int main(int argc, char *argv[]) 
{
    int rank, source, len, size;
    char *bufin, *bufout;
    
    rank = source = len = size = -1;

    if (OSMP_Init(&argc, &argv) != OSMP_SUCCESS) {
        printf("OSMP_INIT Error: OSMP_Init \n");
        exit(EXIT_FAILURE);
    }

    if (OSMP_Size(&size) != OSMP_SUCCESS || size!=2) {
        printf("OSMP_SIZE Error\n");
        exit(EXIT_FAILURE);
    }

    if (OSMP_Rank(&rank) != OSMP_SUCCESS) {
        printf("OSMP_RANK Error: OSMP_Rank \n");
        exit(EXIT_FAILURE);
    }

    if (rank == 0) {
        bufin = malloc(32);
        rand_str(bufin, 32);
        printf("process %d sending random message to process %d: %s\n", rank, 1, bufin);
        if (OSMP_Send(bufin, 32, 1) != OSMP_SUCCESS) {
            printf("OSMP_SEND Error: OSMP_Send \n");
            exit(EXIT_FAILURE);
        }
        free(bufin);
    } 
    else if(rank==1){
        sleep(5);
        bufout = malloc(16);
        printf("Process %d trying to receive msg\n", rank);
        if (OSMP_Recv(bufout, 16, &source, &len) == OSMP_SUCCESS) {
            printf("Process %d received %d bytes from process %d: %s\n", rank, len, source, bufout);
            printf("OSMP_RECV: Error expected, Receive-buffer smaller than msg-length \n");
            exit(EXIT_FAILURE);
        }
        free(bufout);

    }
    if(OSMP_Finalize()!=OSMP_SUCCESS){
        printf("OSMP_FINALIZE Error");
        exit(EXIT_FAILURE);
    }
    
	if(rank==1){
		 printf("#04 ############################ SUCCESS\n");
	}
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
