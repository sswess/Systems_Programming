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
	
	//zero out struct
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
	
	//initializing socket
	
	 //int sockfd, newsockfd, portno, clilen;
    //struct sockaddr_in serv_addr;
	//struct sockaddr_in cli_addr;
	printf("done.\nInitializing socket... ");
	int sockfd = -1;	// file descriptor for our server socket
	int fd = -1;	// file descriptor for a client socket
	int portno = atoi(argv[1]);	// server port to connect to
	socklen_t clilen = -1;	// utility variable - size of clientAddressInfo below
	int n = -1;			// utility variable - for monitoring reading/writing from/to the socket
	char buffer[256];	// char array to store data going to and coming from the socket

	struct sockaddr_in serverAddressInfo;	
	struct sockaddr_in clientAddressInfo;
	 
/** If the user gave enough arguments, try to use them to get a port number and address **/
	 
	// try to build a socket .. if it doesn't work, complain and exit
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
	{
       fflush(stdout);
	   perror("ERROR opening socket");
	}
	
/** We now have the port to build our server socket on .. time to set up the address struct **/

	// zero out the socket address info struct .. always initialize!
	bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));

	// set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
	serverAddressInfo.sin_port = htons(portno);
	 
	// set a flag to indicate the type of network address we'll be using  
    serverAddressInfo.sin_family = AF_INET;
	
	// set a flag to indicate the type of network address we'll be willing to accept connections from
    serverAddressInfo.sin_addr.s_addr = INADDR_ANY;
    
/** We have an address struct and a socket .. time to build up the server socket **/
    
	printf("done.\nBinding to port %d... ", portno);
	
    // bind the server socket to a specific local port, so the client has a target to connect to      
    if (bind(sockfd, (struct sockaddr *) &serverAddressInfo, sizeof(serverAddressInfo)) < 0)
	{
		fflush(stdout);
		close(sockfd);
		perror("\nERROR on binding");
		exit(0);
	}
	
	printf("\nWaiting for client... \n");
	fflush(stdout);
	
	clilen = sizeof(clientAddressInfo);
	
	if (listen( sockfd, 100 ) == -1 ){
		printf( "\nERROR, listen() failed (%s : %d)\n", __FILE__, __LINE__ );
		fflush(stdout);
		close( sockfd );
		exit(0);
	}
	//fd = accept(sockfd, (struct sockaddr *)&clientAddressInfo, &clilen );
	while(1){
		fd = accept(sockfd, (struct sockaddr *)&clientAddressInfo, &clilen );
		printf("here");
		fflush(stdout);
		
		
		
		cpid = fork();
		if(cpid == 0){
			session(portno, fd);
			wait(&cpid);
			printf("session ended %d\n",cpid);
			break;
		}else {
			close(fd);
			
			continue;
		}
	
		//sleep(20);
	}
	
	
	if((fd == accept(sockfd, (struct sockaddr *)&clientAddressInfo, &clilen )) == -1 ){
	fflush(stdout);
	perror("ERROR on accept()");
	close(sockfd);
	}
	
}
