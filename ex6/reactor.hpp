# include "poll.h"
# include "pthread.h"
#include <cstdlib>
#include <cstdio>
#define INIT 10; //first init value of capacity variable

typedef void*(*func)(void *);

typedef struct reactor_unit{
    pollfd *pfd;
    func* f;
    pthread_t thread;
}reactor_unit;

typedef struct reactor{
    reactor_unit* reactors;
    int avail; //taken reactor_units
    int capacity; //capacity of reactor_units
} reactor;

void* newReactor();
void InstallHandler(reactor* r, func* f, int fd);
void RemoveHandler(reactor* r, int fd);