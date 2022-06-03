CC = g++

CFLAGS  = -g -Wall


all: main client singleton reactor pollserver

main: main1.cpp
	$(CC) -pthread $(CFLAGS) main1.cpp -o main

client: client.cpp
	$(CC) -pthread $(CFLAGS) client.cpp -o client

singleton: singleton.cpp
	$(CC) -pthread $(CFLAGS) singleton.cpp -o singleton

reactor: reactor.cpp
	$(CC) -pthread $(CFLAGS) reactor.cpp -o reactor

pollserver: pollserver.cpp
	$(CC) -pthread $(CFLAGS) pollserver.cpp -o pollserver

pollclient: pollclient.cpp
	$(CC) -pthread $(CFLAGS) pollclient.cpp -o pollclient

clean:
	rm main client singleton reactor pollserver