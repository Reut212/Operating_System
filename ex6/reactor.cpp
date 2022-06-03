#include "reactor.hpp"

void* newReactor(){
    reactor *rec = (reactor *) malloc(sizeof(reactor));
    rec->avail=0;
    rec->capacity=INIT;
    rec->reactors= (reactor_unit *) malloc(rec->capacity * sizeof(reactor_unit));
    return (void*)rec;
}

void InstallHandler(reactor* r, func* f, int fd){
    if (r->capacity==r->avail){ //not available space for a new reactor_unit
        r->capacity*=2;
        r->reactors =(reactor_unit *) realloc(r->reactors,r->capacity * sizeof(reactor_unit));
    }
    r->reactors[r->avail].pfd->fd=fd;
    r->reactors[r->avail].f=f;
    r->reactors[r->avail].thread= pthread_create(&r->reactors[r->avail].thread, NULL, *f, NULL);
    r->avail++;
}

void RemoveHandler(reactor* r, int fd) {
    for (int i = 0; i < r->avail; i++) {
        if (r->reactors[i].pfd->fd == fd) {
            r->avail--;
            r->reactors[i].pfd = NULL;
            r->reactors[i].f = NULL;
            pthread_cancel(r->reactors[i].thread);
            break;
        }
    }
}
