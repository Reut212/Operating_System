#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // for close
#include <netdb.h>
#include <pthread.h>
#include <sys/types.h>
#define PORT "3490"
pthread_mutex_t lock;

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int create_connection(){
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo("neta", PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure
    return sockfd;
}

void *push_client(void *arg) {
    pthread_mutex_lock(&lock);
    int clientSocket = *((int *) arg);
    int self = pthread_self();
    char snum[5];
    sprintf(snum,"%d",self);
    if (send(clientSocket, "PUSH", 4, 0) == -1)
        perror("send");
    if (send(clientSocket, snum, 5, 0) == -1)
        perror("send");
    close(clientSocket);
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

//void *pop_client(void *arg) {
//    int clientSocket = *((int *) arg);
//    if (send(clientSocket, "POP", 3, 0) == -1) {
//        perror("send");
//    }
//    close(clientSocket);
//    pthread_exit(NULL);
//}


int main() {
    pthread_t PUSHING[5];
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    for (int i = 0; i < 5; i++) {  // main accept() loop
        int clientSocket= create_connection();
        if (pthread_create(&PUSHING[++i], NULL, push_client, &clientSocket) != 0)
            printf("Failed to create thread\n");
        pthread_join(PUSHING[i], NULL);
    }
//    sleep(2);
//    int top = create_connection();
//    if (send(top, "TOP", 3, 0) == -1)
//        perror("send");
//    char buf[1024];
//    if ((recv(top, buf, 1024, 0)) == -1)
//        perror("recv");
//    else {
//        int num = atoi(buf);
//        if (num != 100){
//            printf("test failed!!!!!!!!");
//            return 0;
//        }
//        else{
//            printf("test passed!!!!!!!!");
//        }
//    }
//    pthread_t POPPING[100];
//    for (int i = 0; i < 100; i++) {  // main accept() loop
//        int clientSocket= create_connection();
//        if (pthread_create(&POPPING[++i], NULL, pop_client, &clientSocket) != 0)
//            printf("Failed to create thread\n");
//        pthread_join(PUSHING[i], NULL);
//    }
//    sleep(2);
//    if (send(top, "TOP", 3, 0) == -1)
//        perror("send");
//    if ((recv(top, buf, 1024, 0)) == -1)
//        perror("recv");
//    else {
//        if (strlen(buf) != 0){
//            printf("test failed!!!!!!!!");
//            return 0;
//        }
//        else{
//            printf("test passed!!!!!!!!");
//        }
//    }
    pthread_mutex_destroy(&lock);
}
