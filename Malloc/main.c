#include <stdio.h>
#include <assert.h>
#include "mymalloc.h"

int main(int argc, char **argv)
{
	void* p1, *p2, *p3, *p4, *p5;
	
	printf("\tTEST 1\n");
	free(p1);
	printf("p1 not yet allocated = bad free attempt, should fail\n\n");
	
	printf("\tTEST 2\n");
	assert((p1 = malloc(10000)) == NULL);
	printf("Attempt to malloc 10000 bytes should fail\n");
	
	assert((p1 = malloc(0)) == NULL);
	printf("Attempt to malloc 0 bytes should fail\n");
	
	assert((p1 = malloc(5000)) == NULL);
	printf("Attempt to malloc 5000 bytes should fail\n");
	
	printf("\tTEST 3\n");
	assert((p1 = malloc(1000)) != NULL);
	printf("Malloced p1 : 1000 at %p\n",p1);
	assert((p2 = malloc(1000)) != NULL);
	printf("Malloced p2 : 1000 at %p\n",p2);
	assert((p3 = malloc(1000)) != NULL);
	printf("Malloced p3 : 1000 at %p\n",p3);
	assert((p4 = malloc(1000)) != NULL);
	printf("Malloced p4 : 1000 at %p\n\n",p4);
	assert((p5 = malloc(2000)) == NULL);
	printf("Attempt to malloc 2000 bytes should fail\n");
	assert((p5 = malloc(100)) != NULL);
	printf("Malloced p5 : 100 at %p\n\n");
	
	printf("\tTEST 4\n");
	free(p1 + 324);
	printf("Attempt to free p1 + 324 = bad free attempt, should fail\n\n");
	
	printf("\tTEST 5\n");
	free(p1);
	printf("Freed p1\n");
	free(p2);
	printf("Freed p2\n");
	free(p3);
	printf("Freed p3\n");
	free(p4);
	printf("Freed p4\n");
	free(p4);
	printf("p4 already freed = Bad free attempt, should fail\n");
	free(p5);
	printf("Freed p4\n\n");
	
	printf("\tTEST 6\n");
	assert((p1 = malloc(1000)) != NULL);
	printf("Malloced p1 : 1000 at %p\n",p1);
	assert((p2 = malloc(1000)) != NULL);
	printf("Malloced p2 : 1000 at %p\n",p2);
	assert((p3 = malloc(1000)) != NULL);
	printf("Malloced p3 : 1000 at %p\n",p3);
	assert((p4 = malloc(1000)) != NULL);
	printf("Malloced p4 : 1000 at %p\n\n",p4);
	
	free(p1);
	printf("Freed p1\n");
	free(p3);
	printf("Freed p3\n");
	free(p4);
	printf("Freed p4\n\n");
	
	printf("\tTEST 7\n");
	assert((p1 = malloc(1000)) != NULL);
	printf("Malloced p1 : 1000 at %p\n",p1);
	
	free(p1);
	printf("Freed p1\n\n");
	
	assert((p1 = malloc(1001)) != NULL);
	printf("Malloced p1 : 1001 at %p\n",p1);
	
	free(p1);
	printf("Freed p1\n");
	
	free(p2);
	printf("Freed p2\n\n");
	
	printf("\tTEST 8\n");
	assert((p1 = malloc(1000)) != NULL);
	printf("Malloced p1 : 1000 at %p\n",p1);
	assert((p2 = malloc(1000)) != NULL);
	printf("Malloced p2 : 1000 at %p\n",p2);
	assert((p3 = malloc(1000)) != NULL);
	printf("Malloced p3 : 1000 at %p\n",p3);
	assert((p4 = malloc(1000)) != NULL);
	printf("Malloced p4 : 1000 at %p\n\n",p4);
	
	free(p1);
	printf("Freed p1\n");
	free(p3);
	printf("Freed p3\n");
	free(p2);
	printf("Freed p2\n");
	
	assert((p1 = malloc(3000)) != NULL);
	printf("Malloced p1 : 3000 at %p\n",p1);
	
	free(p1);
	printf("Freed p1\n");
	
	assert((p1 = malloc(2000)) != NULL);
	printf("Malloced p1 : 2000 at %p\n",p1);
	
	free(p1);
	printf("Freed p1\n");
	
	free(p4);
	printf("Freed p4\n");
	
	/*printf("mallocing p 25000\n");
    void *p = malloc(25000);
	printf("p 25000 '%p'\n", p);
	printf("mallocing p2 1000\n");
    void *p2 = malloc(1000);
    printf("p2 1000 '%p'\n", p2);
    printf("mallocing p4 should be success\n");
    void *p4 = malloc(2000);
    printf("p4 2000 '%p'\n", p4);
    printf("Freeing p4\n");
    free(p4);

    printf("mallocing p3 600\n");
    void *p3 = malloc(600);
    printf("p3 600 '%p'\n", p3);
    printf("mallocing p4 200\n");
    p4 = malloc(200);
    printf("p4 200 '%p'\n", p4);

    printf("Freeing p\n");
    free(p);
    printf("Freeing p2\n");
    free(p2);
    printf("Freeing p3\n");
    free(p3);
    printf("Freeing p4: %p\n",p4);
    free(p4);
    printf("Freeing p3\n");
    free(p3);
*/


}