CC = g++

CFLAGS  = -g -Wall


all: main client singleton pollserver pollclient guard test

main: main1.cpp
	$(CC) -pthread $(CFLAGS) main1.cpp -o main

client: client.cpp
	$(CC) -pthread $(CFLAGS) client.cpp -o client

singleton: singleton.cpp
	$(CC) -pthread $(CFLAGS) singleton.cpp -o singleton

guard: guard.cpp
	$(CC) -pthread $(CFLAGS) guard.cpp -o guard

test: test.cpp
	$(CC) -pthread $(CFLAGS) test.cpp -o test

pollserver: pollserver.cpp
	$(CC) -pthread $(CFLAGS) pollserver.cpp reactor.cpp -o pollserver

pollclient: pollclient.cpp
	$(CC) -pthread $(CFLAGS) pollclient.cpp reactor.cpp -o pollclient

clean:
	rm main client singleton pollserver pollclient guard test