#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // for close
#include <netdb.h>
#include <sys/types.h>
#define PORT "3490"
#include <stdlib.h>

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

void push_client(int num) {
    int clientSocket = create_connection();
    char snum[20];
    sprintf(snum, "%d", num);
    printf("client %d connected\n", num);
    if (send(clientSocket, "PUSH", 6, 0) == -1)
        perror("send");
    if (send(clientSocket, snum, 1024, 0) == -1)
        perror("send");
    printf("client %d pushed %s to the stack\n", num, snum);
    if (send(clientSocket, "EXIT", 6, 0) == -1)
        perror("send");
    printf("client %d disconnected\n", num);
    close(clientSocket);
}

void pop_client(int num) {
    int clientSocket = create_connection();
    printf("client %d connected\n", num);
    if (send(clientSocket, "POP", 6, 0) == -1) {
        perror("send");
    }
    printf("client %d popped from stack\n", num);
    if (send(clientSocket, "EXIT", 6, 0) == -1) {
        perror("send");
    }
    printf("client %d disconnected\n", num);
    close(clientSocket);
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
    while(i<5){
        if (!fork()) { // this is the child process
            push_client(i);
            exit(1);
        }
        sleep(2);
        i++;
    }

    printf("\n"
           "╔╦╗╔═╗╔═╗╔╦╗  ╔═╗╦ ╦╔═╗╦ ╦  ╔═╗╔═╗╔═╗╔═╗╔═╗╔╦╗  ┬\n"
           " ║ ║╣ ╚═╗ ║   ╠═╝║ ║╚═╗╠═╣  ╠═╝╠═╣╚═╗╚═╗║╣  ║║  │\n"
           " ╩ ╚═╝╚═╝ ╩   ╩  ╚═╝╚═╝╩ ╩  ╩  ╩ ╩╚═╝╚═╝╚═╝═╩╝  o\n");
    sleep(2);
    printf("-------------------checking TOP-------------------\n");
    int top = create_connection();
    if (send(top, "TOP", 6, 0) == -1)
        perror("send");
    char buf[1024];
    if ((recv(top, buf, 1024, 0)) == -1)
        perror("recv");
    else {
        int num = atoi(buf);
        if (num != 4){
            printf("test failed!!!!!!!! top is %d\n", num);
            return 0;
        }
        else{
            printf("Top is 4!\n");
            printf("\n"
                   "╔╦╗╔═╗╔═╗╔╦╗  ╔╦╗╔═╗╔═╗  ╔═╗╔═╗╔═╗╔═╗╔═╗╔╦╗  ┬\n"
                   " ║ ║╣ ╚═╗ ║    ║ ║ ║╠═╝  ╠═╝╠═╣╚═╗╚═╗║╣  ║║  │\n"
                   " ╩ ╚═╝╚═╝ ╩    ╩ ╚═╝╩    ╩  ╩ ╩╚═╝╚═╝╚═╝═╩╝  o\n");

        }
        if ((send(top, "EXIT", 6, 0)) == -1)
            perror("recv");
    }
    printf("-------------------checking POP-------------------\n");
    i=0;
    while(i<5){
        if (!fork()) { // this is the child process
            pop_client(i);
            exit(1);
        }
        sleep(2);
        i++;
    }
    printf("\n"
           "╔╦╗╔═╗╔═╗╔╦╗  ╔═╗╔═╗╔═╗  ╔═╗╔═╗╔═╗╔═╗╔═╗╔╦╗  ┬\n"
           " ║ ║╣ ╚═╗ ║   ╠═╝║ ║╠═╝  ╠═╝╠═╣╚═╗╚═╗║╣  ║║  │\n"
           " ╩ ╚═╝╚═╝ ╩   ╩  ╚═╝╩    ╩  ╩ ╩╚═╝╚═╝╚═╝═╩╝  o\n");
    sleep(2);

    printf("-------------------checking TOP-------------------\n");
    top = create_connection();
    if (send(top, "TOP", 6, 0) == -1)
        perror("send");
    if ((recv(top, buf, 1024, 0)) == -1)
        perror("recv");
    else {
//        int num = atoi(buf);
        if (strlen(buf)!=0){
            printf("test failed!!!!!!!!\n");
            return 0;
        }
        else{
            printf("Top is empty!\n");
            printf("\n"
                   "╔╦╗╔═╗╔═╗╔╦╗  ╔╦╗╔═╗╔═╗  ╔═╗╔═╗╔═╗╔═╗╔═╗╔╦╗  ┬\n"
                   " ║ ║╣ ╚═╗ ║    ║ ║ ║╠═╝  ╠═╝╠═╣╚═╗╚═╗║╣  ║║  │\n"
                   " ╩ ╚═╝╚═╝ ╩    ╩ ╚═╝╩    ╩  ╩ ╩╚═╝╚═╝╚═╝═╩╝  o\n");

        }
        if ((send(top, "EXIT", 6, 0)) == -1)
            perror("recv");
    }
    return 0;
}