# include "poll.h"
# include "pthread.h"
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>

typedef void*(func)(void *);

typedef struct reactor_unit{
    pollfd pfd;
    func* f;
    pthread_t thread;
}reactor_unit;

typedef struct reactor{
    reactor_unit reactors[1000];
    int avail; //taken reactor_units
    int capacity; //capacity of reactor_units
} reactor;

void* newReactor();
void InstallHandler(reactor* r, func* f, int fd);
void RemoveHandler(reactor* r, int fd);