# include "Queue.h"

node* newNode(void* k)
{
    node* temp = (node*)malloc(sizeof(node));
    temp->data = k;
    temp->next = NULL;
    return temp;
}

void* createQ(){
    queue* q = (queue*)malloc(sizeof(queue));
    q->front = q->rear = NULL;
    void* the_queue = (void*)q;
    return the_queue;
}

void destoryQ(void* q){
    queue* the_queue = (queue*)q;
    while (the_queue->front!=NULL){
        deQ(q);
    }
    free(q);
}

void enQ(void* q, void* n){
    node* temp = newNode(n);
    queue* the_queue = (queue*)q;
    if (the_queue->rear == NULL) { //empty queue
        the_queue->front = the_queue->rear = temp;
        return;
    }
    node* curr = the_queue->rear;
    the_queue->rear->next = temp;
    the_queue->rear = temp;
    temp->prev = curr;
}

void deQ(void* q){
    queue* the_queue = (queue*)q;
    if (the_queue->front == NULL)
        return;

    // Store previous front and move front one node ahead
    node* temp = the_queue->front;

    the_queue->front = the_queue->front->next;
    // If front becomes NULL, then change rear also as NULL
    if (the_queue->front == NULL)
        the_queue->rear = NULL;
    else{
        the_queue->front->prev = NULL;
    }

    free(temp);
}

int main(){
    void* q = createQ();
    enQ(q, (void*)1);
    enQ(q, (void*)2);
    enQ(q, (void*)3);
    enQ(q, (void*)4);
    enQ(q, (void*)5);
    deQ(q);
    destoryQ(q);
    return 0;
}