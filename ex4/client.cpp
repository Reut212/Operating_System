#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


#define SERVER_PORT 3490
#define SERVER_IP_ADDRESS "10.0.2.15"
#define SIZE 1024

int send_file(FILE *fp, int sockfd) {
    char data[SIZE] = {0};
    int total = 0;
    while (fgets(data, SIZE, fp) != NULL) {
        int sent = send(sockfd, data, sizeof(data), 0);
        if (sent == -1) {
            perror("[-] Error in sending data");
        } else if (sent == 0) {
            perror("Destination is closed");
        } else {
            total = total + sent;
        }
        bzero(data, SIZE);
    }
    return total;
}


int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        printf("Could not create socket : %d", errno);
    }

    struct sockaddr_in serverAddress;
    //struct initialization
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    // convert to big endian system
    serverAddress.sin_port = htons(SERVER_PORT);

    //to binary
    int rval = inet_pton(AF_INET, (const char *) SERVER_IP_ADDRESS, &serverAddress.sin_addr);

    if (rval <= 0) {
        printf("inet_pton() failed");
        return -1;
    }

    //connect
    if (connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        printf("connect() failed with error code : %d", errno);
    } else printf("connected to server\n");

    char data[1024];
    printf("Enter string: ");
    scanf("%s", data);
    send(sock, data, 1024, 0);
    close(sock);
    return 0;
}