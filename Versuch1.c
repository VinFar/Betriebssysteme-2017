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
#include <semaphore.h>

int shmsetpids(void *shm_ptr1, pid_t * pids, int pcount);
pid_t createChild(int argc, char *argv[],sem_t * sem);



int main(int argc, char *argv[]){

	pid_t *child_pid;
	int anz,shm_size;
	void *shm_ptr;
	sem_t * sem_child;
	shm_header *header;
	shm_postbox *postbox;

	if(argc < 2){
	
		perror("Syntax Error!");
		return OSMP_ERROR;
	
	}

	anz = atoi(argv[1]);	//convert second  argument to integer
	
	if(anz<0){
		perror("number of processes can't be lower than 0!");
		return OSMP_ERROR;
	}
	

	child_pid = (pid_t*)malloc((size_t)anz * sizeof(pid_t));		
	
	if(child_pid==0){
		printf("Malloc Error: %s",strerror(errno));
		return OSMP_ERROR;
	}
	
	sem_child = sem_open(SEMNAME, O_CREAT | O_EXCL, 0644, 0);
	
	if(SEM_FAILED == sem_child){
		
		printf("Semaphore Error: %s\n",strerror(errno));
		
	}
	
	if(shm_config(&shm_ptr,argv[2]+1,anz,&shm_size) == OSMP_ERROR){
		perror("SHM_CONFIG ERROR!");
		return OSMP_ERROR; // Init des Shared Memorys
	}

	
	for(int i=0;i<anz;i++){	
	
		child_pid[i] = createChild(argc,&argv[2],sem_child);
		
	}
	
	
	

	if(shmsetpids(shm_ptr, child_pid, anz)!=OSMP_SUCCESS){
		perror("SET_PID ERROR!");
		return OSMP_ERROR;
	}
	
	for(int i=0; i < anz+1; i++){
	
		if(sem_post(sem_child)==-1){
			printf("SEM_POST Nr. %d ERROR: %s",i,strerror(errno));
			return OSMP_ERROR;
		}
	}
	
	//Sync with childrocesses
	
	pid_t tmp;	
	for (int i = 0; i < anz; i++) {
		tmp = waitpid(child_pid[i], NULL, WCONTINUED);
		if(tmp == -1){	
			printf("waitpid_error:%s\n",strerror(errno));
			return OSMP_ERROR;	
		}else{	
		//~ printf("waitpid success:%d\n",child_pid[i]);
		}
	}
	
	header = (shm_header*)shm_ptr;
	postbox = (shm_postbox*)&((char*) shm_ptr)[header->off_postbox];

		pid_t *index;
	index = (pid_t*)shm_ptr;

	//~ sleep(5);

	for(int i=0;i<11;i++){	//SHM ausgeben
		
		printf("%d	",index[i]);
		
	}
	
	printf("\n\n");
	
	for(int k=0;k<anz;k++){
		for(int i=11 + (sizeof(shm_postbox)*k/4);i<11 + (sizeof(shm_postbox)*(k+1)/4);i++){	//SHM ausgeben
		
		printf("%d	", index[i]);
		
	}
	printf("\n\n\n");
}
	
	for(int k=0;k<50;k++){	//SHM ausgeben
		for(int i=(anz+1)*sizeof(shm_postbox)/4 + k*6 + 11;i<(anz+1)*sizeof(shm_postbox)/4 + (k+1)*6 +11;i++){
		printf("%d	",index[i]);
	}
	printf("Nachricht Nr.: %d\n",k);
	}
	
	

	
	sem_destroy(&postbox[FREESLOTS].used_sem);		//Semaphoren zerstören
	sem_destroy(&postbox[FREESLOTS].free_sem);
	sem_destroy(&postbox[FREESLOTS].mutex_sem);

	if(munmap(shm_ptr,(size_t)shm_size)==-1){
		printf("MUNMAP ERROR: %s",strerror(errno));
		return OSMP_ERROR;
	}
	
	if(sem_unlink(SEMNAME)==-1){
		printf("SEM_UNLINK ERROR: %s",strerror(errno));
		return OSMP_ERROR;
	}
	sem_close(sem_child);
	free(child_pid);
	
return OSMP_SUCCESS;

}

int shmsetpids(void *shm_ptr1, pid_t * pids, int pcount)
{

	shm_header *header;
	char * shm_ptr = (char *) shm_ptr1;
	
	header = (shm_header*)shm_ptr;

	memcpy(&shm_ptr[header->off_rank], pids, sizeof(pid_t) * (size_t)pcount);
	//Fehlerüberprüfung

	return OSMP_SUCCESS;
}


pid_t createChild(int argc, char *argv[],sem_t * sem){
	(void)argc;
	pid_t pid;
	
	pid = fork();

	if(pid<0){
		printf("Fehlerbeandlung...");
	}
	if(pid==0){
		char * const a[] ={"a","b","cfghj",NULL};			//Kindprozess

		sem = sem_open(SEMNAME, O_EXCL);

		if(sem_wait(sem) == -1){
		
		printf("SEM_WAIT ERROR: %s",strerror(errno));
			
		}
		
		execvp(argv[0],argv);
		
		printf("FEHLER!!!\n");
		return OSMP_ERROR;
		
	}
	if(pid > 0){
			//printf("Elternprozess\n");	//Elternprozess
		
	}
	
return pid;

}


