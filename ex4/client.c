#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "funcs.h"

#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main()
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
//
//    if (argc != 2) {
//        fprintf(stderr,"usage: client hostname\n");
//        exit(1);
//    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

//    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
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

    while (1){
        printf("Please enter action (PUSH/TOP/POP/EXIT)\n");
        char action[6];
        scanf("%s", action);
        if (strcmp(action, "EXIT") == 0){
            if (send(sockfd, "EXIT", 4, 0) == -1)
                perror("send");
            break;
        }
        else if (strcmp(action, "PUSH") == 0){
            if (send(sockfd, "PUSH", 4, 0) == -1)
                perror("send");
            char data[1024];
            printf("please enter the string you want to push: ");
            scanf("%s", data);
            printf("first: %s\n", data);
            if (send(sockfd, data, 1024, 0) == -1)
                perror("send");
        }
        else if (strcmp(action, "POP") == 0) {
            if (send(sockfd, "POP", 3, 0) == -1) {
                perror("send");
            }
        }
        else if (strcmp(action, "TOP") == 0) {
            if (send(sockfd, "TOP", 3, 0) == -1)
                perror("send");
            char buf[1024];
            if ((recv(sockfd, buf, 1024, 0)) == -1)
                perror("recv");
            else {
                printf("Top of the stack is '%s'\n", buf);
            }
        }
        else{
            printf("Invalid action, please try again or exit!\n");
        }
    }
    close(sockfd);

    return 0;
}