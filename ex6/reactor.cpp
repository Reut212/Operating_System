#include "reactor.hpp"

void* newReactor(){
    reactor *rec = (reactor *) malloc(sizeof(reactor));
    rec->avail=0;
    rec->capacity=3;
    for (int i=0; i<rec->capacity;i++){
        rec->reactors[i].pfd.fd=-1;
    }
    return (void*)rec;
}

void InstallHandler(reactor* r, func* f, int fd){
    if (r->avail==r->capacity){
        perror("no available space!");
        return;
    }
    for (int i=0; i<r->capacity; i++){
        if (r->reactors[i].f==NULL){ //found space
            r->avail++;
            r->reactors[i].pfd.fd=fd;
            r->reactors[i].pfd.events=POLLIN;
            r->reactors[i].f=f;
            int *index = (int*)(char*)malloc(sizeof(char));
            *index = i;
            pthread_create(&r->reactors[i].thread, NULL, f,index);
            return;
        }
    }
}

void RemoveHandler(reactor* r, int fd) {
    for(int i=0; i<r->capacity; i++){
        if (r->reactors[i].pfd.fd==fd){
            r->reactors[i].pfd.fd=NULL;
            r->reactors[i].f=NULL;
            pthread_cancel(r->reactors[i].thread);
            r->avail--;
            return;
        }
    }
}

//// simple example
//void* f(void*){}
//int main(){
//    reactor* reac = (reactor*)newReactor();
//    int fd = open("example.txt", O_RDWR);
//    InstallHandler(reac,f,fd); //place it in 0'th cell
//    InstallHandler(reac,f,fd); //place it in 1'th cell
//    RemoveHandler(reac,fd); //remove it from 0'th cell
//    InstallHandler(reac,f,fd); //place it in 0'th cell
//    printf("");
//}