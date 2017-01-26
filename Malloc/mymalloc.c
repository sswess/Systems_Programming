#include <stdio.h>
#include <string.h>
#include "mymalloc.h"

static char myblock[5000];
const size_t header_size = sizeof(Header);
const size_t short_size = sizeof(short);

// Helper function for mymalloc. Used if data is allocated into a free block that is larger than the space allocated
void add_block(size_t block_size, int location, int index){
	location += header_size;
	memmove(location + header_size, location, header_size*(table_size - index));
	((Header*)(location))->size = ((Header*)(location - header_size))->size - block_size;
	((Header*)(location - header_size))->size = block_size;
	((Header*)(location))->address = ((Header*)(location - header_size))->address + block_size;
	((Header*)(location))->tag = 1;
	table_size++;
}

void *mymalloc(size_t block_size, char* file_name, int line_number){
	int location = short_size + myblock; // Starts from address of start of table
	short index = 0;
	int address = 0; //Address to be returned
	
	//Pre-checks
	if(block_size == 0){
		printf("ERROR: Cannot create block of size 0\n");
		return NULL;
	}
	if(block_size > (5000 - (header_size + short_size))){ //Check if block_size exceeds max possible size
		printf("ERROR: Block is too large for memory: %p\n",block_size);
		return NULL;
	}
	
	// Step 1: Check for available memory space
	// Step 2: Grab address to store data in; create new header entry in table if needed
	// Step 3: Return address
	if(table_size > 0){ //If there is at least 1 element in table, iterate through table
//NOTE: Modularize and write alternate method?
	//Alternate: Select smallest free block that fits block_size
		do{
//printf("%d\n",((Header*)(location))->size);
			if(((Header*)(location))->tag == 1 //Check if block is free space
					&& ((Header*)(location))->size >= block_size){ //Check if size of block is large enough
				if(((Header*)(location))->size != block_size){
					add_block(block_size, location, index);
				}
				address = ((Header*)(location))->address; //Save address of block
				((Header*)(location))->tag = 0; //Set tag to "occupied";
				break;
			}
			location += header_size;
			index++;
		}while(index < table_size);
		
		if(address == 0){ //Table does not contain large enough block, attempt to add entry at end; at least 1 entry exists
			if((((Header*)(location - header_size))->address - location) < (header_size + block_size)){ //Check if enough space is available
				printf("ERROR: Not enough memory available for %d bytes\n",block_size);
				return NULL;
			}
			table_size++;
			address = ((Header*)(location - header_size))->address - block_size; //Address of new block = address of previous block - size of new block
			((Header*)(location))->address = address;
			((Header*)(location))->size = block_size;
			((Header*)(location))->tag = 0;
		}
	}
	else { //Initialize first block and first table entry
		address = myblock + 4999 - block_size;
		//Initialize first header entry of table
		((Header*)(location))->address = address;
		((Header*)(location))->size = block_size;
		((Header*)(location))->tag = 0;
		table_size++;
//printf("myblock = %p, myblock + 4999 = %p\n",myblock, myblock + 4999);
	}
//printf("New memory block created: %p\n",address);
	return address;
}

//Helper function for myfree; merges adjacent empty blocks
void merge(int location, int index){
	int new_size = ((Header*)(location))->size;
//printf("%d\n",new_size);
	int rest = header_size*(table_size - index); //The memory size of all entries after the deleted entry
	
	int dist = 0; // distance to shift entries (max = 2*header_size)
	//Check next entry
	if(index != (table_size - 1) && ((Header*)(location + header_size))->tag == 1){
		dist += header_size;
		new_size += ((Header*)(location + header_size))->size;
//printf("%d : %d\n",((Header*)(location + header_size))->size, index);
		table_size--;
	}
	//Check previous entry
	if(table_size != 1 && ((Header*)(location - header_size))->tag == 1){
		dist += header_size;
		new_size += ((Header*)(location - header_size))->size;
//printf("%d : %d\n",((Header*)(location - header_size))->size, index);
		location -= header_size;
		index--; 
		table_size--;
	}

	//Merge entries
	((Header*)(location))->size = new_size; //Modify free block size to reflect new size after merging
//printf("%d : %d\n",((Header*)(location))->size, index);
	location += header_size;
	memmove(location, location + dist, rest);

//printf("%d : %d\n",index, table_size);
	//If table only contains a free block, remove the block entry
	//If last block is free, remove the block entry
	if(table_size == 1 || index == (table_size - 1)){
		table_size--;
	}

}

void *myfree(void *x, char* file_name, int line_number){
	printf("# of allocated blocks before free = %d\n",table_size); //Used for analysis only
	//Iterate through table to find address
		//Check for validity
	if(!(x < (void*)(myblock + header_size + short_size) || x > (void*)(myblock + 4999))){
		if(!(table_size == 0)){
			int index = 0;
			int location = myblock + short_size;
			//Iterate
			do{
//printf("%p : %d : %d\n",((Header*)(location))->address, ((Header*)(location))->tag, table_size);
				if(((Header*)(location))->address == x){
					((Header*)(location))->tag = 1;
//printf("Removed block at address %p\n",((Header*)(location))->address);
					merge(location, index);
					break;
				}
				index++;
				location += header_size;
			}while(index < table_size);
		}
		else{//NOTE: Technically, should not be reachable
			printf("ERROR: No occupied blocks exist\n");
		}
	}
	else{ 
		printf("ERROR: Address %p is invalid\n",x);
	}
	
	printf("# of allocated blocks after free = %d\n",table_size); //Used for analysis only
}