# include "Queue.hpp"



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
    if (pthread_mutex_init(&q->lock, nullptr) != 0)
    {
        perror("mutex init failed");
    }
    pthread_cond_init(&q->cond, NULL);
    void* the_queue = (void*)q;
    return the_queue;
}

void destoryQ(void* q){
    queue* the_queue = (queue*)q;
    pthread_mutex_lock(&the_queue->lock);
    while (the_queue->front!=nullptr){
        deQ(q);
    }
    free(q);
    pthread_mutex_unlock(&the_queue->lock);
    pthread_mutex_destroy(&the_queue->lock);
}

void enQ(void* q, void* n){
    queue* the_queue = (queue*)q;
    pthread_mutex_lock(&the_queue->lock);
    node* temp = newNode(n);
    if (the_queue->rear == nullptr) { //empty queue
        the_queue->front = the_queue->rear = temp;
        pthread_mutex_unlock(&the_queue->lock);
        return;
    }
    node* curr = the_queue->rear;
    the_queue->rear->next = temp;
    the_queue->rear = temp;
    temp->prev = curr;
    pthread_mutex_unlock(&the_queue->lock);
}

void* deQ(void* q){
    queue* the_queue = (queue*)q;
    pthread_mutex_lock(&the_queue->lock);
    if (the_queue->front == nullptr) {
        pthread_cond_wait(&the_queue->cond, &the_queue->lock);
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
    pthread_mutex_unlock(&the_queue->lock);
    return res;
}

void printQ(void* q){
    queue* the_queue = (queue*)q;
    pthread_mutex_lock(&the_queue->lock);
    node* curr = the_queue->front;
    while (curr!=NULL){
        printf("%s", (char*)curr);
        curr = curr->next;
    }
    pthread_mutex_unlock(&the_queue->lock);
}