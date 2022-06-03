#include "reactor.hpp"

void* newReactor(){
    reactor *rec = (reactor *) malloc(sizeof(reactor));
    rec->count=0;
    for (int i=0; i<MAX_FD; i++){
        rec->pfds[i]=NULL;
    }
    return (void*)rec;
}

void InstallHandler(reactor* r, func* f, int fd){
    if (r->count == MAX_FD){
        perror("not available space");
        return;
    }
    for (int i=0; i<MAX_FD; i++){
        if (r->pfds[i]==NULL){ //found available cell
            r->count++;
            r->pfds[i]->fd = fd;
            r->pfds[i]->events=POLLIN;
            r->funcs[i] = f;
            pthread_t t;
            r->threads[i] = t;
            pthread_create(&t, NULL, *f, NULL);
            break;
        }
    }
}

void RemoveHandler(reactor* r, int fd){
    for (int i=0; i<MAX_FD; i++){
        if (r->pfds[i]->fd==fd){
            r->count--;
            r->pfds[i]=NULL;
            r->funcs[i] = NULL;
            pthread_cancel(r->threads[i]);
            break;
        }
    }
