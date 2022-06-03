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

#define PORT "9034"   // Port we're listening on
reactor* r = (reactor*) newReactor();

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Return a listening socket
int get_listener_socket(void)
{
    int listener;     // Listening socket descriptor
    int yes=1;        // For setsockopt() SO_REUSEADDR, below
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

    for(p = ai; p != NULL; p = p->ai_next) {
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

void* listen_func(void* args){
    for(;;) {
        int poll_count = poll((pollfd*)(r->reactors), r->capacity, -1);

        if (poll_count == -1) {
            perror("poll");
            exit(1);
        }

//        // Run through the existing connections looking for data to read
//        for(int i = 0; i < fd_count; i++) {
//
//            // Check if someone's ready to read
//            if (pfds[i].revents & POLLIN) { // We got one!!
//
//                if (pfds[i].fd == listener) {
//                    // If listener is ready to read, handle new connection
//
//                    addrlen = sizeof remoteaddr;
//                    newfd = accept(listener,
//                                   (struct sockaddr *)&remoteaddr,
//                                   &addrlen);
//
//                    if (newfd == -1) {
//                        perror("accept");
//                    } else {
//                        add_to_pfds(&pfds, newfd, &fd_count, &fd_size);
//
//                        printf("pollserver: new connection from %s on "
//                               "socket %d\n",
//                               inet_ntop(remoteaddr.ss_family,
//                                         get_in_addr((struct sockaddr*)&remoteaddr),
//                                         remoteIP, INET6_ADDRSTRLEN),
//                               newfd);
//                    }
//                } else {
//                    // If not the listener, we're just a regular client
//                    int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);
//
//                    int sender_fd = pfds[i].fd;
//
//                    if (nbytes <= 0) {
//                        // Got error or connection closed by client
//                        if (nbytes == 0) {
//                            // Connection closed
//                            printf("pollserver: socket %d hung up\n", sender_fd);
//                        } else {
//                            perror("recv");
//                        }
//
//                        close(pfds[i].fd); // Bye!
//
//                        del_from_pfds(pfds, i, &fd_count);
//
//                    } else {
//                        // We got some good data from a client
//
//                        for(int j = 0; j < fd_count; j++) {
//                            // Send to everyone!
//                            int dest_fd = pfds[j].fd;
//
//                            // Except the listener and ourselves
//                            if (dest_fd != listener && dest_fd != sender_fd) {
//                                if (send(dest_fd, buf, nbytes, 0) == -1) {
//                                    perror("send");
//                                }
//                            }
//                        }
//                    }
//                } // END handle data from client
//            } // END got ready-to-read from poll()
//        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
}

// Main
int main(void){
    int listener;     // Listening socket descriptor

    int newfd;        // Newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;

    char buf[256];    // Buffer for client data

    char remoteIP[INET6_ADDRSTRLEN];

    // Set up and get a listening socket
    listener = get_listener_socket();

    if (listener == -1) {
        fprintf(stderr, "error getting listening socket\n");
        exit(1);
    }

    InstallHandler(r, listen_func, listener);

    return 0;
}