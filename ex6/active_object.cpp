# include "Queue.hpp"

typedef void* (*beforeFun)(void*);
typedef void (*afterFun)(void*);

typedef struct ao{
    queue* q;
    beforeFun beforePtr;
    afterFun afterPtr;
    pthread_t thread;
} ao;

void * run(void *arg){
    ao* active_object = (ao*)arg;
    while (1){
        void* element = deQ(active_object->q);
        active_object->afterPtr(active_object->beforePtr(element));
    }
}

void destroyAO(ao* active_obj){
    pthread_cancel(active_obj->thread);
}

ao newAO(void* q, beforeFun beforePtr, afterFun afterPtr){
    queue* the_queue = (queue*) q;
    pthread_t t;
    ao* active_obj = (ao*)malloc(sizeof(ao));
    active_obj->q=the_queue;
    active_obj->beforePtr = beforePtr;
    active_obj->afterPtr = afterPtr;
    if (pthread_create(&t, NULL, run, &active_obj) != 0)
        perror("Failed to create thread");
    active_obj->thread = t;
    return *active_obj;
}