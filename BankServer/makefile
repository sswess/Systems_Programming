all: run_server run_client

test: test_server test_client

test_server: testserver.c
	gcc -g -o test_server testserver.c

test_client: testclient.c
	gcc -g -o test_client testclient.c

run_server: server.o cli_session.o
	gcc -g -o run_server server.o cli_session.o

server.o: server.c shmem.h
	gcc -c server.c shmem.h

cli_session.o: cli_session.c shmem.h
	gcc -c cli_session.c shmem.h

run_client: client.c
	gcc -g -o run_client client.c

clean:
	rm *.o
