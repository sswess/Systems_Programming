#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ipc.h>
#include <netdb.h> 





#include "shmem.h"

int exit_flag;

//Helper function for "process_input" to start an account session
int start_session(char* name, Account*ba, char* output){
	int i;
	for(i = 0;i<20;i++){ //
		if(strcmp(ba[i].name, name) == 0)
		{
			//printf("\n\tyou accessed account #%d named:%s\n",i,ba[i].name);
			//sprintf(output, "you accessed account #%d named:%s\n",i,ba[i].name);
			if(ba[i].flag == 1)
			{
				sprintf(output, "Account \"%s\" is in use.", name);
				return -1;
			}
			else{
				ba[i].flag = 1;
				sprintf(output, "Now logged into account \"%s\"", ba[i].name);
				return i;
			}
		}
	}
	sprintf(output, "The account \"%s\" does not exist.", name); 
	return -1;
}

//Helper function for "process_input" to open a new account
void create_account(char* name, Account*ba, char* output){
	if(ba == NULL){
		printf("Accounts array is null");
	}
	//creating account
	int i;
	for(i = 0; i < 20; i++)
	{
		if(strcmp(ba[i].name, "") == 0){
			strncpy(ba[i].name, name, 100);
			sprintf(output, "Created bank account \"%s\"\n",ba[i].name);
			fflush(stdout);
			return;
		}
	}
	sprintf(output, "Unable to create account \"%s\". Server is full.", name);
	//return -1;
}

//Helper function to process the client request
void process_input(char * input, SharedMem* shmem, char* output){
	//write(1, output, sprintf(output, "%s\n", input));
	
	bzero(output, 256);
	Account* ba = shmem->accounts;
	char command[10];
	char argument[100];
	sscanf(input, "%s %s", command, argument);
	
	
	static int currentaccount = -1;
	if(strcmp(command, "start") == 0){
		if(currentaccount == -1){
			currentaccount = start_session(argument, shmem->accounts, output);
		}
		else{
			sprintf(output, "Cannot open new session while currently in session (\"%s\")",ba[currentaccount].name);
		}
	}
	else if(strcmp(command, "finish") == 0){
		if(currentaccount > -1){
			ba[currentaccount].flag = 0;
			sprintf(output, "Logged out of account \"%s\"", ba[currentaccount].name);
			currentaccount = -1;
		}
		else {
			sprintf(output, "You are not currently in a session. Cannot close session.");
		}
	}
	else if(strcmp(command, "credit") == 0){
		if(currentaccount == -1){
			sprintf(output, "You are not currently in a session. Cannot credit an account.");
		}
		else if(atoi(argument) <= 0){
			sprintf(output, "Invalid credit amount of %f specified.", atof(argument));
		}
		else if(ba[currentaccount].flag == 1){
			sprintf(output, "New balance for account \"%s\" = %f", ba[currentaccount].name, ba[currentaccount].balance += atof(argument));
		}
	}
	else if(strcmp(command, "debit") == 0){
		float amt;
		if(currentaccount == -1){
			sprintf(output, "You are not currently in a session. Cannot credit an account.");
		}
		else if((amt = atof(argument)) < 0){
			sprintf(output, "Invalid debit amount of %f specified.", amt);
		}
		else if(ba[currentaccount].flag == 1){
			sprintf(output, "New balance for account \"%s\" = %f", ba[currentaccount].name,
				((ba[currentaccount].balance < (float)amt) ? (ba[currentaccount].balance = 0.0) : (ba[currentaccount].balance -= (float)amt)));
		}
		else {sprintf(output, "You have not logged into account \"%s\".", ba[currentaccount].name);}
	}
	else if(strcmp(command, "open") == 0){
		create_account(argument, shmem->accounts, output);
	}
	else if(strcmp(command, "balance") == 0){
		if(currentaccount == -1){
			sprintf(output, "You are not currently in a session. Cannot check account balance.");
		}
		else {
			sprintf(output, "Balance for account \"%s\" = %f", ba[currentaccount].name, ba[currentaccount].balance);
		}
	}
	else if(strcmp(command, "exit") == 0){
		if(currentaccount != -1){
			ba[currentaccount].flag = 0;
			
		}
		sprintf(output, "EXIT");
	}
	else{
		//write(1, output, sprintf(output, "%s\n",input));
		snprintf(output, 256, "Bad input.\n");
		//write(1, output, 100);
	}
	
}

//Allows clean exit with ctrl-c input
void sigint_handler(int sig){
	exit_flag = !exit_flag;
}

//Main session process
	//Connect to shared memory
	//Create socket
	//Bind to user specified port
	//Connect to client
	//Processing loop to handle client requests
void session(int portno, int fd)
{

	printf("done.\nConnecting to shared memory... ");
	printf("%d",fd);
	fflush(stdout);
	//Shared mem vars
	key_t key = SHMEM_KEY;
	int shmid;
	SharedMem* shmem;
	char err_output[50];
	
	if ((shmid = shmget(SHMEM_KEY, (size_t)sizeof(SharedMem), 0666)) < 0) {
		fflush(stdout);
		sprintf(err_output,"\nERROR shmget (%d:%s:%d)",getpid(),__FILE__,__LINE__);
		perror(&err_output);
        exit(1);
    }
	if ((int)(shmem = (SharedMem*)shmat(shmid, NULL, 0)) == -1) {
        fflush(stdout);
		sprintf(err_output,"\nERROR shmat (%d:%s:%d)",getpid(),__FILE__,__LINE__);
		perror(&err_output);
        exit(1);
    }
	
		printf("done.\nConnected to client!!!!!!!!.\n");
		fflush(stdout);
	
		signal(SIGINT, &sigint_handler);
		printf("Ctrl-c to quit.\n");
	
		//Set socket to non-blocking file descriptor
		//int flags = fcntl(fd, F_GETFL, 0);
		//fcntl(fd, F_SETFL, flags | O_NONBLOCK);
		
		char	request[2048];
		char 	response[2048];
		char	message[256];
		char	output[256];
		exit_flag = 0;
		while(!exit_flag){
				
			printf(">");
			bzero(request, sizeof(request));
	
		
			while(read(fd, request, sizeof(request)) < 1){sleep(1);}
			printf("\nClient> %s",request);
			fflush(stdout);
			bzero(output, sizeof(output));
			//printf("here!!!\n");
				fflush(stdout);
			process_input(request, shmem, output);
			printf("-> %s\n",output);
			fflush(stdout);
			write(fd, output, sizeof(output));
			if(strcmp(output,"EXIT") == 0){
				printf("Client session exiting, closing server...");
				exit_flag = !exit_flag;
			}
			fflush(stdout);
		}
		close(fd);
	
	
}