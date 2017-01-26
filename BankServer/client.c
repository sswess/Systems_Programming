#include <stdio.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <netdb.h>


int main(int argc, char *argv[])
{
	// Declare initial vars
    int sockfd = -1;																// file descriptor for our socket
	int portno = -1;																// server port to connect to
	int n = -1;																		// utility variable - for monitoring reading/writing from/to the socket
	char buffer[256];															// char array to store data going to and coming from the server
    struct sockaddr_in serverAddressInfo;						// Super-special secret C struct that holds address info for building our socket
    struct hostent *serverIPAddress;									// Super-special secret C struct that holds info about a machine's address
    
	
	
	// If the user didn't enter enough arguments, complain and exit
    if (argc < 3)
	{
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
	
	
	
	/** If the user gave enough arguments, try to use them to get a port number and address **/

	// convert the text representation of the port number given by the user to an int
	portno = atoi(argv[2]);
	
	// look up the IP address that matches up with the name given - the name given might
	//    BE an IP address, which is fine, and store it in the 'serverIPAddress' struct
    serverIPAddress = gethostbyname(argv[1]);
    if (serverIPAddress == NULL)
	{
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
				
	// try to build a socket .. if it doesn't work, complain and exit
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
	{
        char full[100];
		sprintf(full, "ERROR, socket() failed (%s : %d)\n", __FILE__,__LINE__);
		perror(full);
		exit(0);
	}


	
	/** We now have the IP address and port to connect to on the server, we have to get    **/
	/**   that information into C's special address struct for connecting sockets                     **/

	// zero out the socket address info struct .. always initialize!
    bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));

	// set a flag to indicate the type of network address we'll be using 
    serverAddressInfo.sin_family = AF_INET;
	
	// set the remote port .. translate from a 'normal' int to a super-special 'network-port-int'
	serverAddressInfo.sin_port = htons(portno);

	// do a raw copy of the bytes that represent the server's IP address in 
	//   the 'serverIPAddress' struct into our serverIPAddressInfo struct
    bcopy((char *)serverIPAddress->h_addr, (char *)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);



	/** We now have a blank socket and a fully parameterized address info struct .. time to connect **/
	
	// try to connect to the server using our blank socket and the address info struct 
	//   if it doesn't work, complain and exit
	
    while (connect(sockfd,(struct sockaddr *)&serverAddressInfo,sizeof(serverAddressInfo)) < 0) 
	{
        char full[100];
		sprintf(full, "ERROR, connect() failed (%s : %d)", __FILE__,__LINE__);
		perror(full);
		sleep(3);
	}	
	
	/** If we're here, we're connected to the server .. w00t!  **/
	
	char	message[256];
	char	output[256];
	
	printf("Connected to server.\n");
	
	int pid = fork();
	
	if(pid == 0){ //Client reads input from client user
		
		int exit_flag = 0;
		while(!exit_flag){
			bzero(message, sizeof(message));
			printf(">");
			fflush(stdout);
			fgets(message, sizeof(message), stdin);
			//printf("here     %d", exit_flag);
			int i;
			for(i = 0; message[i] != '\0'; i++)
				message[i] = tolower(message[i]);
			if(strcmp(message, "exit") == 0){
				printf("%s\n", message);
				fflush(stdout);
				exit_flag = !exit_flag;}
			write(sockfd, message, sizeof(message));
			//printf("here     %d", exit_flag);
		}
		printf("here%d", exit_flag);
		fflush(stdout);
		close(sockfd);
		exit(1);
	}
	else { //Parent reads input from server 
		int exit_flag = 0;
		while(!exit_flag){
			bzero(message, sizeof(message));
			read(sockfd, message, sizeof(message));
			printf("> %s\n",message);
			fflush(stdout);
			if(strcmp(message, "EXIT") == 0){
				printf("%s\n",message);
				fflush(stdout);
				exit_flag = !exit_flag;
			}
		}
		printf("here1     %d", exit_flag);
		fflush(stdout);
		close(sockfd);
		kill(pid, SIGTERM);
		exit(1);
	}
}