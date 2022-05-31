CC = g++

CFLAGS  = -g -Wall


all: main client

main: main.cpp
	$(CC) -pthread $(CFLAGS) main.cpp -o main

client: client.cpp
	$(CC) -pthread $(CFLAGS) client.cpp -o client

clean:
	rm main client