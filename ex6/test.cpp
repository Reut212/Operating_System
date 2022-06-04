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
#include <stdlib.h>
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

    if ((rv = getaddrinfo("reut", PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
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

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *) p->ai_addr),
               s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure
    return sockfd;

}

void *push_client(void* arg) {
    int clientSocket = create_connection();
    int num = *((int *) arg);
    char data[1024];
    sprintf(data, "%d", num);
    printf("client %d connected\n", num);
    while (1) {
        printf("Please enter STRING OR 'EXIT' to exit\n");
        memset(data, 0, 1024);
        scanf("%s", data);
        if (strcmp(data, "EXIT") == 0) {
            if (send(clientSocket, "EXIT", 6, 0) == -1)
                perror("send");
            break;
        } else {
            if (send(clientSocket, "STRING", 6, 0) == -1)
                perror("send");
            if (send(clientSocket, data, 1024, 0) == -1)
                perror("send");
            memset(data, 0, 6);
            if ((recv(clientSocket, data, 1024, 0)) == -1)
                perror("recv");
            bool is_alive = false;
            for (size_t i = 0; i < strlen(data); i++) {
                if (('A' <= data[i] && data[i] <= 'Z') || ('a' <= data[i] && data[i] <= 'z')) {
                    is_alive = true;
                }
                else {
                    printf("not a valid string -- Try Again:\n");
                    is_alive = false;
                    break;
                }
            }
            if(is_alive){
                printf("You're new string is: %s\n",data);
            }
        }
    }
    printf("client %d disconnected\n", num);
    close(clientSocket);
    pthread_exit(NULL);
}


int main() {
    printf("\n"
           "   ____               _______        _           \n"
           "  / __ \\             |__   __|      | |        _ \n"
           " | |  | |_   _ _ __     | | ___  ___| |_ ___  (_)\n"
           " | |  | | | | | '__|    | |/ _ \\/ __| __/ __|    \n"
           " | |__| | |_| | |       | |  __/\\__ \\ |_\\__ \\  _ \n"
           "  \\____/ \\__,_|_|       |_|\\___||___/\\__|___/ (_)\n"
           "                                                 \n"
           "                                                 \n");
    printf("-------------------checking PUSH-------------------\n");
    int i = 0;
    pthread_t PUSHING[5];
    while(i < 5){  // main accept() loop
        if (pthread_create(&PUSHING[i], NULL, push_client, &i) != 0)
            printf("Failed to create thread\n");
        sleep(25);
        i++;
    }
    printf("\n"
           "╔╦╗╔═╗╔═╗╔╦╗  ╔═╗╦ ╦╔═╗╦ ╦  ╔═╗╔═╗╔═╗╔═╗╔═╗╔╦╗  ┬\n"
           " ║ ║╣ ╚═╗ ║   ╠═╝║ ║╚═╗╠═╣  ╠═╝╠═╣╚═╗╚═╗║╣  ║║  │\n"
           " ╩ ╚═╝╚═╝ ╩   ╩  ╚═╝╚═╝╩ ╩  ╩  ╩ ╩╚═╝╚═╝╚═╝═╩╝  o\n");
    return 0;
}