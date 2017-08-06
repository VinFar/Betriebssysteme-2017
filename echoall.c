#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "OSMPLib.h"


int main(int argc, char *argv[]) {

	OSMP_Request test1;

    for (int i = 0; i < argc; i++) {
        //~ printf("Argument %d:%s\n", i, argv[i]);
    }
    printf("\n");

    int size1, rank;

    char test;

    OSMP_Init(&argc, &argv);

    OSMP_Size(&test);

    OSMP_Rank(&rank);
    //~ printf("Rank: %d\n\n", rank);

    test = 'c';
    
	size1 = sizeof(char);
	
	int count=size1;
    
    if (rank > 1) {

        for (int i = 0; i < 1; i++) {
			
            OSMP_Send(&test, size1, 1);
            printf("SEND %d\n",i);
			//~ OSMP_Isend(&test,(int)count,3,&test1);
        }
    }

    
	sleep(1);

    if (rank == 1) {
       
       printf("PRC 1 Finalize!");
       OSMP_Finalize();
       return 0;
               //~ for(int i=0;i<250;i++){
			//~ printf("recv\n");
        //~ OSMP_Recv( &test, count, &rank,  &size1);
     //~ }
        
        

        printf("empfangen: %d\n", test);
        printf("von: %d\n", rank);
        printf("size: %d\n", size1);
    }

	
	

	
    //~ printf("echoall!\n\n");
    
    sleep(2);
	OSMP_Finalize();
    return 0;

}
