CC = g++

CFLAGS  = -g -Wall


all: stack server client

stack: stack.cpp stack.h
	$(CC) $(CFLAGS) -o stack stack.cpp stack.h

server: multi_threaded_server.cpp
	$(CC) -pthread $(CFLAGS) -o server multi_threaded_server.cpp

client: client.cpp
	$(CC) -pthread $(CFLAGS) -o client client.cpp

clean:
	rm stack server client