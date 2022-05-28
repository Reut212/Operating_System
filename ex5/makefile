CC = gcc

CFLAGS  = -g -Wall


all: server client

#stack: stack.cpp stack.h
#	$(CC) $(CFLAGS) -o stack stack.cpp stack.h

server: multi_threaded_server.c
	$(CC) $(CFLAGS) -o server multi_threaded_server.c

client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm server client