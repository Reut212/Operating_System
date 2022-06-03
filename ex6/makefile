CC = g++

CFLAGS  = -g -Wall


all: main client singleton reactor

main: main1.cpp
	$(CC) -pthread $(CFLAGS) main1.cpp -o main

client: client.cpp
	$(CC) -pthread $(CFLAGS) client.cpp -o client

singleton: singleton.cpp
	$(CC) -pthread $(CFLAGS) singleton.cpp -o singleton

reactor: reactor.cpp
	$(CC) -pthread $(CFLAGS) reactor.cpp -o reactor

clean:
	rm main client singleton reactor