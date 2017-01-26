all: test

test: main.o mymalloc.o
	gcc -g -Wall -o test main.o mymalloc.o

libmalloc.a: mymalloc.o
	ar -r libmalloc.a mymalloc.o

mymalloc.o: mymalloc.c mymalloc.h
	gcc -w -c mymalloc.c mymalloc.h

main.o: main.c mymalloc.h
	gcc -w -c main.c mymalloc.h

clean:
	rm *.o
