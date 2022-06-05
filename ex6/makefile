CC=g++
FLAGS=-pthread -fPIC
OBJECTS= guard.o reactor.o
HEADERS=$(wildcard *.hpp)

all: main1 client test singleton pollclient pollserver lib.so guard

lib.so: $(OBJECTS)
	$(CC) --shared -fPIC -pthread $(OBJECTS) -o lib.so

main1: main1.o
	$(CC) $(FLAGS) main1.o -o main1

main1.o: $(HEADERS) main1.cpp
	$(CC) $(FLAGS) -c main1.cpp

client: client.o
	$(CC) $(FLAGS) client.o -o client

client.o: $(HEADERS) client.cpp
	$(CC) $(FLAGS) -c client.cpp

test: test.o
	$(CC) $(FLAGS) test.o -o test

test.o: $(HEADERS) test.cpp
	$(CC) $(FLAGS) -c test.cpp

guard: guard.o
	$(CC) $(FLAGS) guard.o -o guard

guard.o: $(HEADERS)  guard.cpp
	$(CC) $(FLAGS) -c guard.cpp

singleton.o: $(HEADERS) singleton.cpp
	$(CC) $(FLAGS) -c singleton.cpp

singleton: singleton.o
	$(CC) $(FLAGS) singleton.o -o singleton

reactor.o: $(HEADERS) reactor.cpp
	$(CC) $(FLAGS) -c reactor.cpp

pollserver.o: pollserver.cpp
	$(CC) $(FLAGS) -c pollserver.cpp

pollserver: pollserver.o reactor.o
	$(CC) $(FLAGS) pollserver.o reactor.o -o pollserver

pollclient.o: pollclient.cpp
	$(CC) $(FLAGS) -c pollclient.cpp

pollclient: pollclient.o
	$(CC) $(FLAGS) pollclient.o  -o pollclient

clean:
	rm -f *.o main1 test client pollclient pollserver a.out *.so singleton guard

