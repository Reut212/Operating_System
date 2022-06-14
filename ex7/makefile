CC = gcc -fPIC -g -w
OBJECTS = myfs.o mystdio.o test.o

all: tests main libmylibc.so

tests: test.o myfs.o  mystdio.o
	$(CC) test.o  mystdio.o myfs.o -o tests

libmylibc.so: $(OBJECTS)
	$(CC) --shared -fPIC -g -pthread $(OBJECTS) -o libmylibc.so

main: main.o  myfs.o  mystdio.o
	$(CC) main.o  mystdio.o myfs.o -o main

%.o: %.c 
	$(CC) -c $< -o $@

clean:
	rm -f *.o tests main libmylibc.so