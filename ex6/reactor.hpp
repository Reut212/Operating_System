# include "poll.h"
# include "pthread.h"
#include <cstdlib>
#include <cstdio>
#define MAX_FD 1000

typedef void*(*func)(void *);

typedef struct reactor{
    struct pollfd *pfds[MAX_FD];
    func* funcs[MAX_FD];
    pthread_t threads[MAX_FD];
    int count;
} reactor;

void* newReactor();
void InstallHandler(reactor* r, func* f, int fd);
void RemoveHandler(reactor* r, int fd);