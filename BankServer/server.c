#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "shmem.h"
	

void main(int argc, char** argv){
	
	pid_t pid = getpid();
	char err_output[256];
	bzero(err_output, 256);
	key_t key = SHMEM_KEY;
	pid_t cpid;
	int array[20];
	int shmid;
	SharedMem* shmem;
	
	printf("Creating shared memory... ");
	
	// If the user didn't enter enough arguments, complain and exit
    if (argc < 2)
	{
		fflush(stdout);
        perror("ERROR, no port provided\n");
        exit(1);
    }
	
	/*if((shmid = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1){
		fflush(stdout);
		sprintf(err_output,"\nERROR shm_open (%d:%s:%d)",getpid(),__FILE__,__LINE__);
		perror(&err_output);
        exit(1);
	}
	else if((shmem = (SharedMem*)mmap(NULL, sizeof(SharedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0)) == -1){
		fflush(stdout);
		sprintf(err_output,"\nERROR mmap (%d:%s:%d)",getpid(),__FILE__,__LINE__);
		perror(&err_output);
        exit(1);
	}*/
	
	
	if ((shmid = shmget(SHMEM_KEY, (size_t)sizeof(struct shared_memory), IPC_CREAT | 0666)) < 0) {
		fflush(stdout);
		sprintf(err_output,"ERROR shmget (%d:%s:%d)",pid,__FILE__,__LINE__);
		perror(&err_output);
        exit(1);
    }
	
	if ((int)(shmem = (SharedMem*)shmat(shmid, NULL, 0)) == -1) {
		fflush(stdout);
		sprintf(err_output,"ERROR shmat (%d:%s:%d)",pid,__FILE__,__LINE__);
		perror(&err_output);
        exit(1);
    }
	
	bzero(shmem, sizeof(SharedMem));
	int i;
	for(i = 0; i < 20; i++){
		strcpy(shmem->accounts[i].name,"");
		shmem->accounts[i].flag = 0;
		shmem->accounts[i].balance = 0.0;
	}
	
	
	printf("done.\nShared memory created.\n");
	printf("Creating session process... ");
	fflush(stdout);
	
	cpid = fork();
	
	if(cpid == 0)
		session(atoi(argv[1]));
	
	wait(&cpid);
	printf("session ended %d\n",cpid);
	
	
}
