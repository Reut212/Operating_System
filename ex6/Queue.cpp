# include "Queue.hpp"

pthread_mutex_t lock;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

node* newNode(void* k)
{
    node* temp = (node*)malloc(sizeof(node));
    temp->data = k;
    temp->next = nullptr;
    return temp;
}

void* createQ(){
    queue* q = (queue*)malloc(sizeof(queue));
    q->front = q->rear = nullptr;
    if (pthread_mutex_init(&lock, nullptr) != 0)
    {
        perror("mutex init failed");
    }
    void* the_queue = (void*)q;
    return the_queue;
}

void destoryQ(void* q){
    pthread_mutex_lock(&lock);
    queue* the_queue = (queue*)q;
    while (the_queue->front!=nullptr){
        deQ(q);
    }
    free(q);
    pthread_mutex_unlock(&lock);
    pthread_mutex_destroy(&lock);
}

void enQ(void* q, void* n){
    pthread_mutex_lock(&lock);
    node* temp = newNode(n);
    queue* the_queue = (queue*)q;
    if (the_queue->rear == nullptr) { //empty queue
        the_queue->front = the_queue->rear = temp;
        pthread_mutex_unlock(&lock);
        return;
    }
    node* curr = the_queue->rear;
    the_queue->rear->next = temp;
    the_queue->rear = temp;
    temp->prev = curr;
    pthread_mutex_unlock(&lock);
}

void* deQ(void* q){
    pthread_mutex_lock(&lock);
    queue* the_queue = (queue*)q;
    if (the_queue->front == nullptr) {
        pthread_cond_wait(&cond, &lock);
    }
    void* res = the_queue->front->data;
    // Store previous front and move front one node ahead
    node* temp = the_queue->front;

    the_queue->front = the_queue->front->next;
    // If front becomes NULL, then change rear also as NULL
    if (the_queue->front == nullptr)
        the_queue->rear = nullptr;
    else{
        the_queue->front->prev = nullptr;
    }

    free(temp);
    pthread_mutex_unlock(&lock);
    return res;
}