CC = g++

CFLAGS  = -g -Wall -pthread


all: pipeline

#queue: Queue.cpp
#	$(CC) $(CFLAGS) -o queue Queue.cpp
#
#active: active_object.cpp
#	$(CC) $(CFLAGS) -o active active_object.cpp Queue.cpp

pipeline: pipeline.cpp
	$(CC) $(CFLAGS) pipeline.cpp active_object.cpp Queue.cpp -o pipeline

clean:
	rm pipeline