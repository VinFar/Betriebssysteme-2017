#ifndef OSMPLib_H

#define OSMPLib_H
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>


#define OSMP_SUCCESS 0
#define OSMP_ERROR -1
#define filename "/shmtest"
#define BUFFERSIZE 12
#define SLOTS 256
#define OSMP_MAX_MESSAGES_PROC 16 
#define OSMP_MAX_SLOTS 256 
#define OSMP_MAX_PAYLOAD_LENGTH 128
#define SEMNAME "SEMMAIN1"
#define FREESLOTS 0
#define POSTBOXSIZE 8
#define SIZESLOT 24
#define THREAD_FLAG_RUN 2
#define THREAD_FLAG_ERROR 0
#define THREAD_FLAG_DONE 1
#define FLAG_INIT 2
#define FLAG_FINAL 3
#define FLAG_FINALIZED -2


typedef struct{
	
	int size;
	int shm_size;
	int off_rank;
	int off_postbox;
	int off_slot;
	
} shm_header;



typedef struct {
	
		int first;
		int last;
		int queue;
		sem_t free_sem;
		sem_t used_sem;
		sem_t mutex_sem;

}shm_postbox;

typedef struct{
	
		int next;
		int src;
		int size;
		char buffer[BUFFERSIZE];
		
}shm_slot;


// lokal!

typedef struct {
				
		int size;
		int rank;
		int * shm_ptr;
		int myshmsize;
        shm_postbox * fd_postbox;		//Pointer auf Anfang von MSG_V
		shm_slot * fd_slot;		//Pointer auf Anfang von MSG
		
}process;


typedef struct {
	pthread_t thread;
	int flag;
	
	const void *send;
	int size;
	int dest;
	void *recieve;
	int *source;
	int *len;
	
}OSMP_Request;


shm_header *shm_h;


int main();
int createSharedMemory(int oflag, off_t size, const char* name, mode_t mode);
int OSMP_Init(int *argc,char ***argv);
int OSMP_Rank(int *rank);
int OSMP_Size(int *size);
int get_slotid(int postbox_prc);
int OSMP_Finalize(void);
void put_slotid(int postbox_prc,int slotid);
int OSMP_Send(const void *buf,int count, int dest);
int OSMP_Wait(OSMP_Request *request);
int OSMP_Test(OSMP_Request *request,int *flag);
int OSMP_Irecv(void *buf,int count,int *source,int *len,OSMP_Request *request);
int OSMP_Isend(const void *buf,int count, int dest,OSMP_Request *request);
void *THR_recv(void *values);
void *THR_send(void *values);
void cleanThread(OSMP_Request *request);


int shm_config(void **shm_ptr,char *name,int count,int *shm_size);





#endif
