#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include "reactor.hpp"

#define PORT "3490"   // Port we're listening on
reactor *r = (reactor *) newReactor();

int newfd;        // Newly accept()ed socket descriptor
struct sockaddr_storage remoteaddr; // Client address
socklen_t addrlen;
char buf[256];    // Buffer for client data
char remoteIP[INET6_ADDRSTRLEN];

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

// Return a listening socket
int get_listener_socket(void) {
    int listener;     // Listening socket descriptor
    int yes = 1;        // For setsockopt() SO_REUSEADDR, below
    int rv;

    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai); // All done with this

    // If we got here, it means we didn't get bound
    if (p == NULL) {
        return -1;
    }

    // Listen
    if (listen(listener, 10) == -1) {
        return -1;
    }

    return listener;
}

int listener = get_listener_socket();    // Listening socket descriptor
void *client_func(void *args) {
    int index = *((int *) args);
    for (;;) {
        int nbytes = recv(r->reactors[index].pfd.fd, buf, sizeof buf, 0);
        int sender_fd = r->reactors[index].pfd.fd;
        if (nbytes <= 0) {
            if (nbytes == 0) {
                // Connection closed
                printf("Connection closed\n");
            } else {
                perror("recv");
            }
            close(r->reactors[index].pfd.fd); // Bye!
            RemoveHandler(r, r->reactors[index].pfd.fd);
        } else {
            // We got some good data from a client
            for (int j = 0; j < r->avail; j++) {
                // Send to everyone!
                int dest_fd = r->reactors[j].pfd.fd;
                // Except the listener and ourselves
                if (dest_fd != listener && dest_fd != sender_fd) {
                    if (send(dest_fd, buf, nbytes, 0) == -1) {
                        perror("send");
                    }
                }
            }
        }
    }
}

void *listen_func(void *args) {
    for (;;) {
        for (int i = 0; i < r->capacity; i++) {
            r->reactors[0].pfd.revents = 0;
        }
        int poll_count = poll((pollfd *) (r->reactors), r->avail, -1);
        if (poll_count == -1) {
            perror("poll");
            exit(1);
        }
        if (r->reactors[0].pfd.revents & POLLIN) {
            if (r->reactors[0].pfd.fd == listener) {
                // If listener is ready to read, handle new connection
                addrlen = sizeof remoteaddr;
                newfd = accept(listener, (struct sockaddr *) &remoteaddr, &addrlen);
                if (newfd == -1) {
                    perror("accept");
                } else {
                    InstallHandler(r, client_func, newfd);
                    pthread_join(r->reactors[0].thread, NULL);
                }
            }
        }
    }
}


// Main
int main(void) {
    if (listener == -1) {
        fprintf(stderr, "error getting listening socket\n");
        exit(1);
    }
    InstallHandler(r, listen_func, listener);
    pthread_join(r->reactors[0].thread, NULL);
    RemoveHandler(r, listener);
    return 0;
}