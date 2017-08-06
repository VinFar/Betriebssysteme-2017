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

#define OSMP_SUCCESS 0
#define OSMP_ERROR -1



typedef struct{ //1024 Bytes
	
		int begin;
		int ende;
	
} MSG_V;

typedef struct {		//136 Bytes
	int length;
	int sender;
	char message[128];
	int next;
}MSG;


// lokal!
typedef struct {	//176 Bytes
		size_t size;
		int rank;
		int fd_glb;
		int myshmsize;
        MSG_V *fd_MSG_V;		//Pointer auf Anfang von MSG
		MSG *fd_MSG;		//Pointer auf Anfang von MSG_V
}process;

// shm
typedef struct {		//136 Bytes
    size_t  size;
    pid_t pids[15];
} SHM_V;










int main();
int createChild(int argc,char **argv[],int rank);
int* MemoryMap(int fd, size_t amount,off_t offset);
int createSharedMemory(int oflag, off_t size, const char* name, mode_t mode);
int OSMP_Write(const char* name, int oflag, mode_t mode);
int shm_config(int argc, char *argv[]);
int OSMP_Init();
int OSMP_Rank(int *rank);
int OSMP_Size(int *size);






#endif
