#ifndef MYMALLOC_H;
#define MYMALLOC_H;
#define size_t unsigned
#define table_size (*((short*)myblock))

#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )

struct header{
	void * address;
	short size;
	short tag; //0 = occupied, 1 = free
};

typedef struct header Header;

void *mymalloc(size_t x, char* file_name, int line_number);

void *myfree(void *x, char* file_name, int line_number);

#endif