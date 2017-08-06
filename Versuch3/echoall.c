#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "OSMPLib.h"


int main(/*int argc, char *argv[]*/){

	/*printf("PID:%u\n",getpid());
	printf("argc:%d\n",argc);
	for(int i=0;i<argc;i++){
	printf("Argument %d:%s\n",i,argv[i]);
	}
	puts("\n");*/
		OSMP_Init();

		sleep(2);
			printf("echoall: %d\n",getpid());
	return 0;
}
