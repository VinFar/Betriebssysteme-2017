#include "OSMPLib.h"


process global;
int init = OSMP_ERROR,final = FLAG_INIT;

int OSMP_Send(const void *buf,int count, int dest){
	int slotid;	
		
		if(init == OSMP_ERROR){
		
		printf("OSMP_Init ERROR: OSMP not initialisiert!\n");	
		return OSMP_ERROR;
	
		}
		
		if(final == FLAG_FINAL){
			
		printf("OSMP_Finalize ERROR: OSMP was finalized!\n");
		return OSMP_ERROR;
			
		}
		
		if(dest > global.size || dest < 1){
		
		printf("OSMP_Send ERROR: Process %d doesn't exist!\n",dest);
		return OSMP_ERROR;	
			
		}
		
		if(count < 0){
			
		printf("OSMP_Send ERROR: Count can't be lower than 0!\n");
		return OSMP_ERROR;
			
		}
		
		if(count > OSMP_MAX_PAYLOAD_LENGTH){
		
		printf("OSMP_Send ERROR: count can't be grater than 256!\n");		
		return OSMP_ERROR;
			
		}
		
		if(buf == NULL){
			
		printf("OSMP_Send ERROR: *buf is NULL-Pointer!\n");
		return OSMP_ERROR;	
		
		}
		
		if(global.fd_postbox[global.rank].first == FLAG_FINALIZED && global.fd_postbox[global.rank].last == FLAG_FINALIZED){
			
			printf("OSMP_Send ERROR: Process %d had been finalized!",dest);
			return OSMP_ERROR;
		}
		int value_sem=-1;
		
		sem_getvalue(&global.fd_postbox[dest].used_sem,&value_sem);
		
		printf("used sem: %d\n",value_sem);
		
		sem_wait(&global.fd_postbox[dest].free_sem);		//Warte auf einen freien Platz
		sem_wait(&global.fd_postbox[FREESLOTS].free_sem);	//Hier auch 
		
		slotid = get_slotid(FREESLOTS);

		global.fd_slot[slotid].src = global.rank;
		global.fd_slot[slotid].size = count;		
		
		memcpy(&global.fd_slot[slotid].buffer,buf,(size_t)count);

		//~ printf("buffer: %s\n",global.fd_slot[slotid].buffer);
		
		put_slotid(dest,slotid);
				
		sem_post(&global.fd_postbox[FREESLOTS].used_sem);
		sem_post(&global.fd_postbox[dest].used_sem);
		
		return OSMP_SUCCESS;
	
}

void put_slotid(int postbox_prc,int slotid){
	//~ put slotid setzt des last element des übergebenen prozesses auf die übegebene slotid.
	//~ put slotid fügt einen slot in die verkettete liste des übergebenen prozesses ein
	
		sem_wait(&global.fd_postbox[postbox_prc].mutex_sem);
	
	if(global.fd_postbox[postbox_prc].first == -1 && global.fd_postbox[postbox_prc].last == -1){
		//~ falls postbox voll WAR!
		global.fd_postbox[postbox_prc].first = slotid;
		
	}else{
		
		global.fd_slot[global.fd_postbox[postbox_prc].last].next=slotid;
	
			
	}
	
		global.fd_postbox[postbox_prc].last=slotid;
		
		sem_post(&global.fd_postbox[postbox_prc].mutex_sem);

} 

int get_slotid(int postbox_prc){
	//~ get slotid gibt das erste freie element des übergebenen prozcccesses zurück
		
		int slotid;
		
		//~ Warte auf Freigabe der MUTEX Semaphore!
		sem_wait(&global.fd_postbox[postbox_prc].mutex_sem);

		slotid = global.fd_postbox[postbox_prc].first;	//Slotid extrahieren

		if(global.fd_postbox[postbox_prc].first == global.fd_postbox[postbox_prc].last ){ 
			
					//~ Falls die Postbox ab dieser nachricht voll ist!																									
					global.fd_postbox[postbox_prc].first=-1;
					global.fd_postbox[postbox_prc].last=-1;
														
			}else{

					global.fd_postbox[postbox_prc].first = global.fd_slot[slotid].next;

					}
					
					global.fd_slot[slotid].next=-1;
		
		sem_post(&global.fd_postbox[postbox_prc].mutex_sem);
	
	return slotid;
	
}

int OSMP_Recv(void *buf,int count, int *source,int *len){
	int slotid;
	
	if(init == OSMP_ERROR){
		
	printf("OSMP_Init ERROR: OSMP not initialisiert!\n");	
	return OSMP_ERROR;
	
	}
	
			if(final == FLAG_FINAL){
			
		printf("OSMP_Finalize ERROR: OSMP was finalized!\n");
		return OSMP_ERROR;
			
		}
	
	if(count > OSMP_MAX_PAYLOAD_LENGTH || count < 0){
	
	printf("OSMP_Recv ERROR: Wrong count length!\n");	
	return OSMP_ERROR;	
		
	}
	
	if(buf == NULL){
		
	printf("OSMP_Recv ERROR: *buf is NULL-Pointer!\n");	
	return OSMP_ERROR;
	}
	
	sem_wait(&global.fd_postbox[global.rank].used_sem);		
	sem_wait(&global.fd_postbox[FREESLOTS].used_sem);
	
	slotid = get_slotid(global.rank);
	
	*source = global.fd_slot[slotid].src;
	*len = global.fd_slot[slotid].size;
	
	
	
	if (*len < count) {

		count = *len;
		

	}

	if(*len > count){
		
		printf("OSMP_ERROR: Lengths not equal!\n");
		
		sem_post(&global.fd_postbox[FREESLOTS].free_sem);
		sem_post(&global.fd_postbox[global.rank].free_sem);
		
		return OSMP_ERROR;
	}
	
	memcpy(buf,&global.fd_slot[slotid].buffer,(size_t)(sizeof(int)));
	
	put_slotid(FREESLOTS,slotid);
	
	sem_post(&global.fd_postbox[FREESLOTS].free_sem);
	sem_post(&global.fd_postbox[global.rank].free_sem);
	
	return OSMP_SUCCESS;

}

int OSMP_Rank(int *rank){			//fertig
	
	if(init == OSMP_ERROR){
		
	printf("OSMP_Init ERROR: OSMP not initialisiert!\n");	
	return OSMP_ERROR;
	
	}
	
			if(final == FLAG_FINAL){
			
		printf("OSMP_Finalize ERROR: OSMP was finalized!\n");
		return OSMP_ERROR;
			
		}
	
	if(global.rank==0){
		
		printf("Rank not defined!\n");
		return OSMP_ERROR;
	
	}

	if(rank == NULL){
	
	printf("OSMP_Rank ERROR: rank is NULL-Pointer!\n");	
	return OSMP_ERROR;	
	}
	
	*rank = global.rank;
	
	return OSMP_SUCCESS;
}

int OSMP_Size(int *size){			//fertig
									//OSMP_Size speichert in *size die Anzahl der Prozesse die gestartet wurden
	if(init == OSMP_ERROR){
		
	printf("OSMP_Init ERROR: OSMP not initialisiert!\n");	
	return OSMP_ERROR;
	
	}
	
			if(final == FLAG_FINAL){
			
		printf("OSMP_Finalize ERROR: OSMP was finalized!\n");
		return OSMP_ERROR;
			
		}
	
	
	if(global.size==0){
		
	printf("Size not defined!\n");	
	return OSMP_ERROR;

	}
	if(size == NULL){
	
	printf("OSMP_Size ERROR: size is NULL-Pointer!\n");	
	return OSMP_ERROR;	
	}
	
	*size = global.size;

	return OSMP_SUCCESS;
}

int OSMP_Init(int *argc,char ***argv){					//fertig

	void * shm_ptr = NULL;
	int shm_fd;
	int shm_size;
	shm_header *header;

	if(*argc < 3){
		
		printf("OSMP_Init ERROR!\n");
		return OSMP_ERROR;
		
	}


	shm_fd = shm_open(*argv[0] +1, O_CREAT | O_RDWR, 0777);					//SHM öffnen um die Anzahl der Prozesse zu lesen
	
	if(shm_fd==-1){
		
			printf("SHM_OPEN ERROR: %s\n",strerror(errno));
			return OSMP_ERROR;
			
	}
	
	shm_ptr = mmap(NULL, sizeof(shm_header), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	
	if(shm_ptr == MAP_FAILED){
		
	printf("MMAP ERROR: %s\n",strerror(errno));
	return OSMP_ERROR;
		
	}
	
	header = (shm_header*)shm_ptr;
	
	shm_size = header->shm_size;
	
	if(munmap(shm_ptr, sizeof(shm_header))){
		
		printf("MUNMAP ERROR: %s\n",strerror(errno));
		return OSMP_ERROR;

	}
	
	shm_ptr = mmap(NULL, (size_t) shm_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, shm_fd, 0);

	if(shm_ptr == MAP_FAILED){
		
		printf("MMAP ERROR: %s\n",strerror(errno));
		return OSMP_ERROR;

	}

	header = (shm_header*)shm_ptr;
	
			pid_t *pid_ptr;
			
			pid_ptr=(pid_t*)(shm_ptr + (int)sizeof(shm_header));	
						
			global.size = header->size;		
			
			global.shm_ptr = (int*)shm_ptr;
	
	pid_t pid = getpid();
	
	for(int i=0;i<global.size;i++){ 								//Hier wird jedem Prozess ein
			
														//interner eindeutiger Rank vergeben
		if(pid_ptr[i] == pid){							//Es werden solange die PID-Felder
														//durchgesucht, bis die eigeine PID
														//gefunden wurde und anhand der
														//Iterationsweite der Rank vergeben
			global.rank = (int)i+1;
			
			break;
		}

		}
		
		global.myshmsize = header->shm_size;
		
		global.fd_postbox = (shm_postbox*)(shm_ptr + header->off_postbox);
		
		global.fd_slot = (shm_slot*)(shm_ptr + header->off_slot);
		
		init = OSMP_SUCCESS;
		
		return OSMP_SUCCESS;
				
}

int OSMP_Finalize(void){
	
	if(init == OSMP_ERROR){
		
		printf("OSMP_Init ERROR: OSMP not initialisiert!\n");	
		return OSMP_ERROR;
	
		}
	
	global.fd_postbox[global.rank].first = global.fd_postbox[global.rank].last = FLAG_FINALIZED;
	
	sem_destroy(&global.fd_postbox[global.rank].used_sem);
	sem_destroy(&global.fd_postbox[global.rank].free_sem);
	sem_destroy(&global.fd_postbox[global.rank].mutex_sem);
	
	global.rank = 0;
	global.fd_postbox = NULL;
	global.fd_slot = NULL;
	global.shm_ptr = NULL;
	global.size = 0;

	final = FLAG_FINAL;

	return OSMP_SUCCESS;
}

int OSMP_Isend(const void *buf,int count, int dest,OSMP_Request *request){
	
	if(init == OSMP_ERROR){
		
	printf("OSMP_Init ERROR: OSMP not initialisiert!\n");	
	return OSMP_ERROR;
	
	}
	
	if(buf == NULL){
	
	printf("OSMP_Isend ERROR: buf is NULL-Pointer!\n");	
	return OSMP_ERROR;
	
	}
	
	if(count > OSMP_MAX_PAYLOAD_LENGTH || count < 0){
		
	printf("OSMP_Isend ERROR: wrong count length!\n");	
	return OSMP_ERROR;
		
	}
	
	if(request == NULL){
		
		printf("OSMP_Isend ERROR: request is NULL-Pointer!\n");
		return OSMP_ERROR;
	}
	
	if(dest < 0){
		
	printf("OSMP_Isend ERROR: dest can't be lower than 0!");
	return OSMP_ERROR;
	
	}
	
	request->size = count;
	request->dest = dest;
	request->send = buf;
	request->flag = THREAD_FLAG_RUN;
	
	pthread_create(&request->thread,NULL,THR_send,request);
	
	return OSMP_SUCCESS;
}

void cleanThread(OSMP_Request *request){
	
	request->flag=THREAD_FLAG_DONE;
	request->send=NULL;
	request->size=0;
	request->dest=0;
	request->recieve=NULL;
	request->source=0;
	request->len=0;
		
	
}

void *THR_send(void *values){

		//~ (OSMP_Request*)values;

		OSMP_Request *req=values;

		if(OSMP_Send(req->send,req->size,req->dest) == OSMP_ERROR){
		
			req->flag=THREAD_FLAG_ERROR;
			
		}else{
			req->flag=THREAD_FLAG_DONE;
		}
	return NULL;
}

void *THR_recv(void *values){
	
		OSMP_Request *req=values;
		
		if(OSMP_Recv(req->recieve,req->size,req->source,req->len)==OSMP_ERROR){
			
			req->flag=THREAD_FLAG_ERROR;
			
		}else{
			
			req->flag=THREAD_FLAG_DONE;
			
		}
return NULL;
}

int OSMP_Irecv(void *buf,int count,int *source,int *len,OSMP_Request *request){
	
	if(init == OSMP_ERROR){
		
	printf("OSMP_Init ERROR: OSMP not initialisiert!\n");	
	return OSMP_ERROR;
	
	}
	
			if(final == FLAG_FINAL){
			
		printf("OSMP_Finalize ERROR: OSMP was finalized!\n");
		return OSMP_ERROR;
			
		}
	
	if(buf == NULL){
		
	printf("OSMP_Irecv ERROR: buf is NULL-Pointer!\n");	
	return OSMP_ERROR;
	
	}
	
	if(count > OSMP_MAX_PAYLOAD_LENGTH || count < 0){
		
	printf("OSMP_Irecv ERROR: count length ERROR!\n");
	return OSMP_ERROR;
		
	}
	
		request->source=source;
		request->recieve = buf;
		request->len = len;
		request->flag = THREAD_FLAG_RUN;
		
		pthread_create(&request->thread,NULL,THR_recv,request);	
		
		return OSMP_SUCCESS;
}

int OSMP_Test(OSMP_Request *request,int *flag){

	if(init == OSMP_ERROR){
		
	printf("OSMP_Init ERROR: OSMP not initialisiert!\n");	
	return OSMP_ERROR;
	
	}
	
			if(final == FLAG_FINAL){
			
		printf("OSMP_Finalize ERROR: OSMP was finalized!\n");
		return OSMP_ERROR;
			
		}
	
	if(request == NULL){
		
		printf("OSMP_Test ERROR: request is NULL-Pointer!\n");
		return OSMP_ERROR;
	}

	*flag=request->flag;
	
	return OSMP_SUCCESS;
}

int OSMP_Wait(OSMP_Request *request){
	if(init == OSMP_ERROR){
		
	printf("OSMP_Init ERROR: OSMP not initialisiert!\n");	
	return OSMP_ERROR;
	
	}
	
	if(request == NULL){
		
		printf("OSMP_Wait ERROR: request is NULL-Pointer!\n");
		return OSMP_ERROR;
	}
	
			if(final == FLAG_FINAL){
			
		printf("OSMP_Finalize ERROR: OSMP was finalized!\n");
		return OSMP_ERROR;
			
		}
	while(request->flag==0);	
	
	return OSMP_SUCCESS;
}

int shm_config(void **shm_ptr,char *name,int count,int *shm_size){	//fertig
	
	int fdshm;
	shm_header *header;
	char * shm_ptr_tmp = *shm_ptr;
	shm_postbox *shmpostbox;
	sem_t sem;
	
	if(count<0){
		perror("Postbox Counter can't be lower than 0!\n");
		return OSMP_ERROR;
	}
	
	fdshm = shm_open(name,O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	
	if(fdshm == -1){
		perror("SHM_OPEN ERROR!\n");
		return OSMP_ERROR;
	}
	
	*shm_size = (off_t)((int)sizeof(shm_header) + count*(int)sizeof(pid_t) + (int)sizeof(shm_postbox)*(count+1) + SLOTS*(int)sizeof(shm_slot));

	if(ftruncate(fdshm, *shm_size) == -1){
		printf("ftruncate Error: %s\n",strerror(errno));
		return OSMP_ERROR;
	}
	
	shm_ptr_tmp = mmap(NULL, (size_t)*shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdshm, 0);
		
	if(shm_ptr_tmp==MAP_FAILED){
		printf("MMAP ERROR: %s\n",strerror(errno));
		return OSMP_ERROR; 	
	}
	
	pid_t *index;
	index = (pid_t*)shm_ptr_tmp;

	for(int i=0;i<1000;i++){	//shm zum test eindeutig bechreiben debug
	
	index[i] = 0;

	}

	if(sem_init(&sem, 1, 0)){
		printf("SEM_INIT ERROR: %s\n",strerror(errno));
		return OSMP_ERROR;
	}

	header = (shm_header*)shm_ptr_tmp;	

	header->shm_size=(int)*shm_size;
	
	header->size = count;
	
	header->off_rank = (int)sizeof(shm_header);
	
	header->off_postbox = (int)(header->off_rank + count*(int)sizeof(pid_t));
	
	header->off_slot = (int)(header->off_postbox + (int)sizeof(shm_postbox)*(count+1));		
		
	global.size = header->size;
	global.myshmsize = header->shm_size;
	global.shm_ptr =shm_ptr_tmp;
	global.fd_postbox = (shm_postbox*)(shm_ptr_tmp + header->off_postbox);
	global.fd_slot = (shm_slot*)(shm_ptr_tmp + header->off_slot);	

	global.fd_postbox[0].first = 0;			//Free-Postbox intialisieren
	global.fd_postbox[0].last = SLOTS-1;
	
	
	
	shmpostbox = global.fd_postbox;//(shm_postbox*)(shm_ptr_tmp + header->off_postbox);
	shmpostbox[FREESLOTS].first = 0;
	shmpostbox[FREESLOTS].last = SLOTS -1 ;
	

	
	if(sem_init(&shmpostbox[FREESLOTS].free_sem, 1, SLOTS)){
		perror("FREE_SEMAPHOR ERROR\n"); return OSMP_ERROR;
	}
	if(sem_init(&shmpostbox[FREESLOTS].used_sem, 1, 0)){
		perror("USED_SEMAPHOR ERROR\n"); return OSMP_ERROR;
	}
	if(sem_init(&shmpostbox[FREESLOTS].mutex_sem, 1, 1)){
		perror("MUTEX_SEMAPHOR ERROR\n"); return OSMP_ERROR;
	}

	for(int i=0;i<SLOTS;i++){ //Verkette Liste initialisieren
		
		global.fd_slot[i].next = i+1;

		
			if(i+1==SLOTS){
			
			global.fd_slot[i].next=-1;	
			
			}
			global.fd_slot[i].size=0;
	}
	
	for(int i=1;i<count+1;i++){			//postbox first und last zunächst auf -1 setzen, da noch keine
										//Nachrichten vorhanden
		
			global.fd_postbox[i].first = -1;
			global.fd_postbox[i].last = -1;
			
			if(sem_init(&shmpostbox[i].free_sem, 1, OSMP_MAX_MESSAGES_PROC)){
			perror("FREE_SEMAPHOR ERROR\n"); return OSMP_ERROR;
			}
			if(sem_init(&shmpostbox[i].used_sem, 1, 0)){
			perror("SEM_Init ERROR!\n"); return OSMP_ERROR;
			}
			if(sem_init(&shmpostbox[i].mutex_sem, 1, 1)){
			perror("SEM_Init Error!\n"); return OSMP_ERROR;
			}			
	}

	*shm_ptr = shm_ptr_tmp;	
	return OSMP_SUCCESS;
	
}

