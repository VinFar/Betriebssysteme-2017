#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "OSMPLib.h"
#include <errno.h>



int main(int argc, char *argv[]){
	int *rank1;
	int *fdtmp;
	(void)rank1;
	(void)fdtmp;
	//printf("rank1: %p\n",rank1);	
	
	
		
	//char * const list[] ={"a","b","cfghj",NULL};	
	
	
	shm_config(argc,argv); // Init des Shared Memorys
	
	
	
	
	//for(int i=0;i<0;i++){ // Prozesse erzeugen
		
	createChild(argc,&argv,0);
	createChild(argc,&argv,0);
	createChild(argc,&argv,0);
	//fdtmp = fd_glb + (sizeof(MSG)*256 + sizeof(MSG_V));
	//fdtmp=fdtmp + 4;
	//fdtmp=0x17;
	//printf("fdtmp: %p\n",fdtmp);
	//OSMP_Rank(rank1);
	
	
	
	//printf("rank: %d\n",anzprc);
	
return 0;
}
