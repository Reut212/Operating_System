CC = gcc

CFLAGS  = -g -Wall


all: shell server

shell: shell.c
	$(CC) $(CFLAGS) -o shell shell.c

server: server.c
	$(CC) $(CFLAGS) -o server server.c

clean:
	rm shell server