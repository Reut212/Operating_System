#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
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

    while (1) {
        printf("Please enter action (STRING/EXIT)\n");
        char action[6];
        memset(action, 0, 6);
        scanf("%s", action);
        if (strcmp(action, "EXIT") == 0) {
            if (send(sockfd, "EXIT", 5, 0) == -1)
                perror("send");
            break;
        } else if (strcmp(action, "STRING") == 0) {
            if (send(sockfd, "STRING", 6, 0) == -1)
                perror("send");
            char data[1024];
            printf("please enter the string you want : ");
            scanf("%s", data);
            if (send(sockfd, data, 1024, 0) == -1)
                perror("send");
            memset(action, 0, 6);
            if ((recv(sockfd, data, 1024, 0)) == -1)
                perror("recv");
            printf("You're new string is: %s\n",data);
        } else {
            printf("Invalid action, please try again or exit!\n");
        }
    }
    close(sockfd);

    return 0;
}
