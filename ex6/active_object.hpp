# include "Queue.hpp"
#ifndef ACTIVE_OBJECT_H_
#define ACTIVE_OBJECT_H_


typedef void* (*beforeFun)(void*);
typedef void* (*afterFun)(void*);

typedef struct ao{
    void* q;
    beforeFun beforePtr;
    afterFun afterPtr;
    pthread_t thread;
} ao;
ao newAO(void* q, beforeFun beforePtr, afterFun afterPtr);
void destroyAO(ao* active_obj);

#endif
