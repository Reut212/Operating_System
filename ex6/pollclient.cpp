#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "reactor.hpp"
#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to
bool bye = false;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

void *write_to_all(void *args) {
    int sock = *((int *) args);
    free(args);
    while (1) {
        printf("Please enter a string to send to everyone 'EXIT' to exit\n");
        char data[1024];
        memset(data, 0, 1024);
        scanf("%s", data);
        if (strcmp(data, "EXIT") == 0) {
            if (send(sock, "EXIT", 5, 0) == -1)
                perror("send");
            close(sock);
            bye = true;
            pthread_exit(NULL);
        } else {
            if (send(sock, data, 1024, 0) == -1)
                perror("send");
            memset(data, 0, 6);
        }
    }
}

void *read_from_all(void *args) {
    int sock = *((int *) args);
    free(args);
    while (1) {
        char buf[1024];
        memset(buf, 0, 1024);
        if ((recv(sock, buf, 1024, 0)) == -1){
            perror("recv");
            pthread_exit(NULL);
        }
        printf("new message: %s", buf);
    }
}


int main() {
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

    pthread_t read, write;
    pthread_create(&read, NULL, read_from_all,&sockfd);
    pthread_create(&write, NULL, write_to_all,&sockfd);
    pthread_join(write, NULL);
    pthread_join(read, NULL);
    while (!bye) {  // when client wants to disconnect
        return 0;
    }
}
