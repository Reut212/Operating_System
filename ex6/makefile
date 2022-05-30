CC = g++

CFLAGS  = -g -Wall -pthread


all: queue active pipeline

queue: Queue.cpp
	$(CC) $(CFLAGS) -o queue Queue.cpp

active: active_object.cpp
	$(CC) $(CFLAGS) -o active active_object.cpp Queue.cpp

pipeline: pipeline.cpp
	$(CC) $(CFLAGS) -o pipeline pipeline.cpp active_object.cpp Queue.cpp

clean:
	rm queue active pipeline