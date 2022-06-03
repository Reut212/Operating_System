CC = g++

CFLAGS  = -g -Wall


all: main client singleton reactor client2 server2

main: main1.cpp
	$(CC) -pthread $(CFLAGS) main1.cpp -o main

client: client.cpp
	$(CC) -pthread $(CFLAGS) client.cpp -o client

singleton: singleton.cpp
	$(CC) -pthread $(CFLAGS) singleton.cpp -o singleton

prog: pollserver pollclient

reactor: reactor.cpp
	$(CC) -pthread $(CFLAGS) reactor.cpp -o reactor

pollserver: pollserver.cpp
	$(CC) -pthread $(CFLAGS) pollserver.cpp reactor.cpp -o pollserver

pollclient: pollclient.cpp
	$(CC) -pthread $(CFLAGS) pollclient.cpp reactor.cpp -o pollclient

clean:
	rm main client singleton reactor server2 client2