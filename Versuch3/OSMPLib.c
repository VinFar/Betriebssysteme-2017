#include "OSMPLib.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>



//const char *shm_filename="/shmtest";

SHM_V *global_SHM_V;
process myprocadr;


pid_t createChild(int argc, char **argv[],int rank){
	pid_t pid;
	pid = fork();
	(void)rank;
	(void)argv;
	(void)argc;
		
	if(pid<0){
		printf("Fehlerbeandlung...");
	}
	if(pid==0){
		char * const a[] ={"a","b","cfghj",NULL};
		size_t i;
		for(i=0;i<global_SHM_V->size;i++){	//Hier wird im process-struct Bereich des SHM
											//nach der ersten freien Stelle für die PID des
											//jeweiligen Prozesses gesucht und anschließend
											//die PID an diese Stelle geschrieben
			if(global_SHM_V->pids[i]==0){
				
				global_SHM_V->pids[i]=getpid();
				
				//printf("pid: %d \n",global_SHM_V->pids[i]);
			
				break;
			
			}
		}
		printf("pid_oben: %d \n",global_SHM_V->pids[i]);

		
		//OSMP_Rank(&rank);
		//printf("rank: %d\n",rank);					//Kindprozess
		execvp("./echoall",a);
		//sleep(2);
	}
	if(pid > 0){
		//printf("Elternprozess\n");	//Elternprozess
	

	}
return pid;
}

int createSharedMemory(int oflag, off_t size, const char* name, mode_t mode){
	int fd,tmp;
	fd = shm_open(name,oflag,mode);
	if(fd == -1){
	printf("shm_open_error:%s\n",strerror(errno));
	}else{
		//printf("filedescriptor: %d\n",fd);
		tmp = ftruncate(fd,size);
		
		if(tmp == -1){
			printf("ftruncate_error:%s\n",strerror(errno));
		}else{
			//printf("ftruncate success!\n");
		}
		
	}
	return fd;
}

int* MemoryMap(int fd,size_t amount,off_t offset){
	int* ptr;
	ptr = mmap(NULL,amount,PROT_READ | PROT_WRITE,MAP_SHARED,fd,offset);
	//printf("sharedmem_pointer: %d\n",*ptr);
	if(*ptr == -1){
			printf("mmap_error:%s\n",strerror(errno));
		}else{
		//printf("mmap success:%p\n",ptr);
		}
		return ptr;
}

int OSMP_Write(const char* name, int oflag, mode_t mode){
	int fd,stat;
	(void)stat;
	fd = shm_open(name,oflag,mode);
	if(fd==-1){
	printf("shm_open_error1:%s\n",strerror(errno));
	}else{
		//puts("shm_open: opened file!");
	}
	return 0;
}

int OSMP_Send(const void *buf,int count, int dest){
	puts("OSMP_Send");
	(void)buf;
	(void)count;
	(void)dest;
	return OSMP_SUCCESS;
	
}

int OSMP_Recv(void *buf,int count, int *source,int *len){
	puts("OSMP_Recieve");
	(void)buf;
	(void)count;
	(void)len;
	(void)source;
	
	return OSMP_SUCCESS;

}

int OSMP_Rank(int *rank){
	pid_t pid = getpid();
	printf("pid: %d\n", pid);
	(void) rank;
	
	int *ptr;
	(void)ptr;
	
	int i=4,tmp;
	(void)i;
	(void)tmp;
	
	return OSMP_SUCCESS;
}

int OSMP_Size(int *size){
	SHM_V *tmpptr;
	int fdtmp;
	fdtmp = shm_open("/shmtest",O_RDONLY,0666);
	tmpptr = (SHM_V *)MemoryMap(fdtmp,sizeof(size_t),0);
	//printf("size: %p",tmpptr);
	*size = (int)tmpptr->size;
	//munmap(fdtmp,sizeof(size_t));
	return OSMP_SUCCESS;
}

int OSMP_Init(){
	
	int amount,anz_prc,i=0;
	(void) amount;
	(void) anz_prc;
	(void) i;
	
	size_t myshmsize;
	
	anz_prc = 9;
	
	SHM_V *shm;
	
	MSG_V *MSG_V_ptr;
	
	MSG *MSG_ptr;
	(void) MSG_ptr;
	(void) MSG_V_ptr;

	myprocadr.fd_glb = shm_open("/shmtest",O_RDWR, 0666); //SHM öffnen um die Anzahl der Prozesse zu lesen
		
	shm = (SHM_V *)MemoryMap(myprocadr.fd_glb,sizeof(size_t),0); //Anzahl der Prozesse auslesen
	
	global_SHM_V=(SHM_V*)shm;		//Pointer von MemoryMap auf global_SHM_V casten
		
	myprocadr.size = shm->size;	//Anzahl in myprocadr.size speichern
	
	//printf("myprocadr: %zu\n\n",myprocadr.size);
	
	munmap(shm,sizeof(size_t));	//SHM wieder schließen um ihn anschließend mit der
								//richtigen GRöße wieder zu öffnen

	myshmsize = sizeof(size_t) + myprocadr.size*sizeof(pid_t) + sizeof(MSG_V)*myprocadr.size + 256*sizeof(MSG);
	
	shm = (SHM_V *)MemoryMap(myprocadr.fd_glb,myshmsize,0);

	pid_t pid = getpid();

	printf("pid_jetzt: %d \n",pid);

	for(int i=0;((size_t)i)<myprocadr.size;i++){
		
		printf("pid_for_schleife: %d\n",global_SHM_V->pids[i]);
		
		if(global_SHM_V->pids[i] == pid){	
		
			myprocadr.rank = i; 
			puts("blaa");
			//printf("iteration: %d",(int)i);
			
			break;
		}
		
		printf("iteration: %d\n",i);
	
	}

		printf("rank:%d\n",myprocadr.rank);

		return OSMP_SUCCESS;
	
		
}

int shm_config(int argc, char *argv[])
{	puts("test1");										//shm_config erstellt und initialisiert den Shared Memory
	int fdshm,anz_prc;
	
	int *shmptr;
	
	unsigned char *fd_tmp;
	(void) fd_tmp;
	
	anz_prc = *argv[1]-48;
	
	off_t myshmsize;
	
	myshmsize = (off_t)(sizeof(size_t) + myprocadr.size*sizeof(pid_t) + sizeof(MSG_V)*myprocadr.size + 256*sizeof(MSG));
    
 	fdshm = createSharedMemory(O_CREAT | O_RDWR, myshmsize,"/shmtest",0777);
	
	shmptr = MemoryMap(fdshm,(size_t)myshmsize,0);
	
	global_SHM_V=(SHM_V*)shmptr;
	
	global_SHM_V->size=(size_t)anz_prc;
	
	for(size_t i=0;i<global_SHM_V->size;i++){
			
		global_SHM_V->pids[i]=0;	
		
		}
	
	//global_SHM_V->pids[0]=6345;
	//global_SHM_V->pids[1]=6346;
	//global_SHM_V->pids[0]=6372;

	//*shmptr=112;
	
	//OSMP_Size(shmptr);
	
	//printf("shmptr:%d\n",*shmptr);
	(void) argc;
	return OSMP_SUCCESS;
	
}


