/* OSMP-Test #05 Labor BS
#05 recv soll blockierd wenn keine Nachricht vorliegt
	->Test ggf schon durch 04 abgedeckt?
R. Mentrup  5/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "settings.h"


void rand_str(char *dest, size_t length);

int main(int argc, char *argv[]) {
	char *bufin /*, *bufout*/;
	int j;
    int source,len,size=-1;
    int rank = -1;

    if (OSMP_Init(&argc, &argv) != OSMP_SUCCESS) {
        printf("OSMP_INIT Error \n");
        exit(EXIT_FAILURE);
    }

    if (OSMP_Size(&size) != OSMP_SUCCESS || size!=1) {
        printf("OSMP_SIZE Error\n");
        exit(EXIT_FAILURE);
    }

    if (OSMP_Rank(&rank) != OSMP_SUCCESS) {
        printf("OSMP_RANK Error \n");
        exit(EXIT_FAILURE);
    }

    if (rank == 0) {
        sleep(5);
        for (j = 1; j < 20; j++) {
            bufin = malloc(32);
            printf("Process %d trying to receive msg %d\n", rank, j);
            if (OSMP_Recv(bufin, 32, &source, &len) != OSMP_SUCCESS) {
                printf("OSMP_RECV Error: OSMP_Recv \n");

                exit(EXIT_FAILURE);
            }
            printf("Process %d received %d bytes from process %d: %s\n", rank, len, source, bufin);
            free(bufin);
        }


    }
    if(OSMP_Finalize()!=OSMP_SUCCESS){
        printf("OSMP_FINALIZE Error");
        exit(EXIT_FAILURE);
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
