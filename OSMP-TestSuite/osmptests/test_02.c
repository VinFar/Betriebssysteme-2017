/* OSMP-Test Labor BS
#02  Verwendung von Funktionalitäten ohne Aufruf von OSMP_Init
R. Mentrup  5/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "settings.h"

int main(int argc, char *argv[]) 
{
    int size=-1, rank=-1;
	const void *sendbuf = "Probe Nachricht #1";
	int recsource, reclen;
	char recbuf[OSMP_MAX_PAYLOAD_LENGTH];

	
	/* OSMP_Init extra vergessen!!  */
    UNUSED(argc); 
	UNUSED(argv);

	
    if (OSMP_Size(&size) != OSMP_ERROR|| size!=-1) {
        printf("#OSMP_SIZE ohne OSMP_Init Aufruf\n");
        exit(EXIT_FAILURE);
    }

    if (OSMP_Rank(&rank) != OSMP_ERROR || rank!=-1) {
        printf("#OSMP_RANK ohne OSMP_Init Aufruf\n");
        exit(EXIT_FAILURE);
    }
	
	if (OSMP_Send(sendbuf, (int)strlen(sendbuf)+1, 0) != OSMP_ERROR){
		printf("Hallo\n");
		printf("#OSMP_Send Erfolgreich ohne OSMP_Init Aufruf!\n");
		exit(EXIT_FAILURE);
	}
	
	if (OSMP_Recv((void *)recbuf, OSMP_MAX_PAYLOAD_LENGTH, &recsource, &reclen) != OSMP_ERROR){
		printf("#OSMP_Recv Erfolgreich ohne OSMP_Init Aufruf!\n");
		exit(EXIT_FAILURE);
	}
  
	/* Für diesen Test wird nur ein prozess benötigt da ein Erfolgreiches Senden/Empfangen nicht möglich sein soll
	if(rank > 0) {
		if (OSMP_Send(sendbuf, (int)strlen(sendbuf)+1, 0) != OSMP_SUCCESS) 
			printf("OSMP_Send Erfolgreich ohne OSMP_Init Aufruf!\n");
		}
	else {
			if (OSMP_Recv((void *)recbuf, OSMP_MAX_PAYLOAD_LENGTH, &recsource, &reclen) != OSMP_ERROR) 
				printf("OSMP_Recv Erfolgreich ohne OSMP_Init Aufruf!\n");
		}
	*/	

	if(OSMP_Finalize()!=OSMP_ERROR){
        printf("OSMP_FINALIZE Erfolgreich ohne OSMP_Init Aufruf\n");
        exit(EXIT_FAILURE);
    }
	
	printf("#02 ############################ SUCCESS\n");
    return EXIT_SUCCESS;
}
