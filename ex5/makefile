CC = gcc

CFLAGS  = -g -Wall


all: server client

#stack: stack.cpp stack.h
#	$(CC) $(CFLAGS) -o stack stack.cpp stack.h

server: multi_threaded_server.cpp
	$(CC) $(CFLAGS) -o server multi_threaded_server.cpp

client: client.cpp
	$(CC) $(CFLAGS) -o client client.cpp

clean:
	rm server client