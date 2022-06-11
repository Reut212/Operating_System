CC = gcc

CFLAGS  = -g -Wall

all: test

myfs: myfs.c
	$(CC) $(CFLAGS) -o myfs myfs.c

test: test.c
	$(CC) $(CFLAGS) -o test myfs.c test.c

clean:
	rm test