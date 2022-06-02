#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
# include "main1.hpp"

#define PORT "3490"  // the port users will be connecting to
#define MAX_SIZE 1024

#define BACKLOG 10   // how many pending connections queue will hold

void *queue1;
void *queue2;
void *queue3;
pipeline* pipe1;

// queue functions
node *newNode(void *k) {
    node *temp = (node *) malloc(sizeof(node));
    temp->data = k;
    temp->next = nullptr;
    return temp;
}

void *createQ() {
    queue *q = (queue *) malloc(sizeof(queue));
    q->front = q->rear = nullptr;
    if (pthread_mutex_init(&q->lock, nullptr) != 0) {
        perror("mutex init failed");
    }
    pthread_cond_init(&q->cond, NULL);
    void *the_queue = (void *) q;
    return the_queue;
}

void destoryQ(void *q) {
    queue *the_queue = (queue *) q;
    pthread_mutex_lock(&the_queue->lock);
    while (the_queue->front != nullptr) {
        deQ(q);
    }
    free(q);
    pthread_mutex_unlock(&the_queue->lock);
    pthread_mutex_destroy(&the_queue->lock);
}

void enQ(void *q, void *n) {
    queue *the_queue = (queue *) q;
    pthread_mutex_lock(&the_queue->lock);
    node *temp = newNode(n);
    if (the_queue->rear == nullptr) { //empty queue
        the_queue->front = the_queue->rear = temp;
        pthread_cond_signal(&the_queue->cond);
        pthread_mutex_unlock(&the_queue->lock);
        return;
    }
    node *curr = the_queue->rear;
    the_queue->rear->next = temp;
    the_queue->rear = temp;
    temp->prev = curr;
    pthread_mutex_unlock(&the_queue->lock);
}

void *deQ(void *q) {
    queue *the_queue = (queue *) q;
    pthread_mutex_lock(&the_queue->lock);
    if (the_queue->front == nullptr) {
        pthread_cond_wait(&the_queue->cond, &the_queue->lock);
    }
    void *res = the_queue->front->data;
    // Store previous front and move front one node ahead
    node *temp = the_queue->front;

    the_queue->front = the_queue->front->next;
    // If front becomes NULL, then change rear also as NULL
    if (the_queue->front == nullptr)
        the_queue->rear = nullptr;
    else {
        the_queue->front->prev = nullptr;
    }

    free(temp);
    pthread_mutex_unlock(&the_queue->lock);
    return res;
}

void *run(void *arg) {
    ao *active_object = (ao *) arg;
    while (1) {
        void *element = deQ(active_object->q);
        active_object->afterPtr(active_object->beforePtr(element));
    }
}

//active object functions

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

//pipeline functions
void enQ_to_queue2(void *n) {
    enQ(queue2, n);
}

void enQ_to_queue3(void *n) {
    enQ(queue3, n);
}

void destroy_message(void *n) {
    free(n);
}

char cesare_cipher_char(char c) {
    if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) {
        if (c == 'z') {
            return 'a';
        } else if (c == 'Z') {
            return 'A';
        } else {
            return (char) ((int) c + 1);
        }
    } else {
        perror("not a valid char");
        return '!';
    }
}

void *caesar_cipher(void *mes) {
    message *m = (message *) mes;
    char *str = (char *) m->data;
    for (size_t i = 0; i < strlen(str); i++) {
        str[i] = cesare_cipher_char(str[i]);
    }
    m->data = str;
    return (void *) m;
}

char small_big_letters_char(char c) {
    if ('A' <= c && c <= 'Z') {
        return (char) ((int) c + 'a' - 'A');
    } else if ('a' <= c && c <= 'z') {
        return (char) ((int) c - 'a' + 'A');
    } else {
        perror("not a valid char");
        return '!';
    }
}

void *small_big_letters(void *mes) {
    message *m = (message *) mes;
    char *str = (char *) m->data;
    for (size_t i = 0; i < strlen(str); i++) {
        str[i] = small_big_letters_char(str[i]);
    }
    m->data = str;
    return (void *) m;
}

void destroy_pipeline(pipeline *p) {
    pipeline *pipe = (pipeline *) p;
    destroyAO(&pipe->ao1);
    destroyAO(&pipe->ao2);
    destroyAO(&pipe->ao3);
    free(p);
}

void *send_response(void *mes) {
    message *m = (message *) mes;
    char *str = (char *) m->data;
    if (send(m->socket, str, 1024, 0) == -1)
        perror("send");
    return (void *) m;
}

message *create_massage(int sock, char *data) {
    message *mes = (message *) malloc(sizeof(message));
    mes->socket = sock;
    mes->data = data;
    return mes;
}

pipeline *create_pipeline() {
    pipeline *p = (pipeline *) malloc(sizeof(pipeline));
    p->ao1 = newAO(queue1, (beforeFun) caesar_cipher, (afterFun) enQ_to_queue2);
    p->ao2 = newAO(queue2, (beforeFun) small_big_letters, (afterFun) enQ_to_queue3);
    p->ao3 = newAO(queue3, (beforeFun) send_response, (afterFun) destroy_message);
    return p;
}

void *socketThread(void *arg) {
    int self = pthread_self();
    int newSocket = *((int *) arg);
    printf("client %d connected\n", self);
    char buf[MAX_SIZE];
    while (1) {
        memset(buf, 0, MAX_SIZE);
        if ((recv(newSocket, buf, 6, 0)) == -1)
            perror("recv");
        if (strcmp(buf, "STRING") == 0) {
            char data[1024];
            if ((recv(newSocket, data, 1024, 0)) == -1)
                perror("recv");
            else {
                message *mes = create_massage(newSocket, data);
                enQ(queue1, mes);
            }
        } else if (strcmp(buf, "EXIT") == 0) {
            printf("client %d disconnected\n", self);
            close(newSocket);
            pthread_exit(NULL);
        }
    }
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

int main(void) {
    queue1 = createQ();
    queue2 = createQ();
    queue3 = createQ();
    pipe1 = create_pipeline();
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");
    pthread_t tid[60];
    int i = 0;
    while (1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *) &their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);

        //for each client request creates a thread and assign the client request to it to process
        //so the main thread can entertain next request
        if (pthread_create(&tid[i++], NULL, socketThread, &new_fd) != 0)
            printf("Failed to create thread\n");
    }
    destroy_pipeline(pipe1);
    destoryQ(queue1);
    destoryQ(queue2);
    destoryQ(queue3);
    return 0;
}