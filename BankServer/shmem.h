#define SHMEM_KEY 654
#define SHM_NAME "bank server"

//typedef enum state{INSESSION, IDLE}State;

typedef struct account{
	char name[100];
	float balance;
	short flag; //0 = occupied, 1 = free
}Account;

typedef struct shared_memory{
	Account accounts[20];
	int shutdown_flag;
} SharedMem;
//1 is shutdown
//void process_input(char * input, SharedMem* shmem, char* output);
void session(int portno);
//void process_input(char * input, SharedMem* shmem, char* output);

