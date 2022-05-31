#include "active_object.hpp"
#include <pthread.h>

void *run(void *arg) {
    ao *active_object = (ao *) arg;
//    queue* the_queue = (queue*)active_object->q;
    while (1) {
        void *element = deQ(active_object->q);
        element = active_object->afterPtr(active_object->beforePtr(element));
    }
}

void destroyAO(ao *active_obj) {
    pthread_cancel(active_obj->thread);
}

ao newAO(void *q, beforeFun beforePtr, afterFun afterPtr) {
    pthread_t t;
    ao *active_obj = (ao *) malloc(sizeof(ao));
    active_obj->q = q;
    active_obj->beforePtr = beforePtr;
    active_obj->afterPtr = afterPtr;
    if (pthread_create(&t, NULL, run, (void *) active_obj) != 0)
        perror("Failed to create thread");
    active_obj->thread = t;
    return *active_obj;
}