CC = gcc

CFLAGS  = -g -Wall


all: queue active pipeline

queue: Queue.cpp
	$(CC) $(CFLAGS) -o queue Queue.cpp

active: active_object.cpp
	$(CC) $(CFLAGS) -o active active_object.cpp

tests: pipeline.cpp
	$(CC) $(CFLAGS) -o pipeline pipeline.cpp

clean:
	rm queue active pipeline