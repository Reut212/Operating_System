CC = g++

CFLAGS  = -g -Wall


all: server client tests

#stack: stack.cpp stack.h
#	$(CC) $(CFLAGS) -o stack stack.cpp stack.h

server: multi_threaded_server.cpp
	$(CC) -pthread $(CFLAGS) -o server multi_threaded_server.cpp funcs.cpp

client: client.cpp
	$(CC) -pthread $(CFLAGS) -o client client.cpp

tests: tests.cpp
	$(CC) -pthread $(CFLAGS) -o tests tests.cpp

clean:
	rm server client tests