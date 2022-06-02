CC = g++

CFLAGS  = -g -Wall


all: main client singleton

main: main.cpp
	$(CC) -pthread $(CFLAGS) main.cpp -o main

client: client.cpp
	$(CC) -pthread $(CFLAGS) client.cpp -o client

singleton: singleton.cpp
	$(CC) -pthread $(CFLAGS) singleton.cpp -o singleton

clean:
	rm main client singleton