#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// queue functions
typedef struct QNode {
    void* data;
    struct QNode* next;
    struct QNode* prev;
} node;

typedef struct Queue {
    struct QNode *front, *rear;
    pthread_mutex_t lock;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
} queue;

void* createQ();
void destoryQ(void* q);
void enQ(void* q,void* n);
void* deQ(void* q);
void printQ(void* q);

//active object functions
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

//pipeline functions
typedef struct pipeline{
    ao ao1;
    ao ao2;
    ao ao3;
} pipeline;

typedef struct massage{
    int socket;
    char* data;
}message;

pipeline* create_pipeline();

void* caesar_cipher(void* s); //first before function
void* small_big_letters(void* s); //second before function
void* send_response(void* s); //third before function

void enQ_to_queue2(void* n); //first after function
void enQ_to_queue3(void* n); //second after function
void destroy_message(void* n);  //third after function

void destroy_pipeline(pipeline* p);