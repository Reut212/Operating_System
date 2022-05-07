CC = gcc

CFLAGS  = -g -Wall


all: server client

#stack: stack.cpp stack.h
#	$(CC) $(CFLAGS) -o stack stack.cpp stack.h

server: multi_threaded_server.c
	$(CC) -pthread $(CFLAGS) -o server multi_threaded_server.c funcs.c

client: client.c
	$(CC) -pthread $(CFLAGS) -o client client.c

clean:
	rm server client