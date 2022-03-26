#include "shell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <netdb.h>
#define MAX 80
#define SA struct sockaddr
#define PORT 3490



int open_connection() {
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Client socket creation failed...\n");
        return 0;
    } else
        printf("Client socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        return 0;
    }

    printf("connected to the server..\n");
    return sockfd;
}


void print_dir_files(){
    DIR *d;
    struct dirent *dir;
    char cwd[1000];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        perror("getcwd() error");
    d = opendir(cwd);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            //Condition to check regular file.
            if(dir->d_type==DT_REG){
                printf("%s\n",dir->d_name);
            }
        }
        closedir(d);
    }
}


void shell() {
    int conn = 0; //is there an open connection ?
    char action[20];
    while (1) {
        printf("yes master? ");
        scanf("%s", action);
        printf("your action is: %s\n", action);
//        if (getcwd(cwd, sizeof(cwd)) == NULL)
//            perror("getcwd() error");
//        else
//            printf("current working directory is: %s\n", cwd);
        if (!strcmp(action, "EXIT")) {
            return;
        } else if (!strcmp(action, "ECHO")) {
            char input[256];
            bzero(input, 256);
            fgets(input, sizeof(input), stdin);
            if (conn) {
                write(conn, input+1, sizeof(input));
            } else printf("%s", input + 1);

        } else if (!strcmp(action, "TCP")) {
            scanf("%s", action);
            if (!strcmp(action, "PORT")) {
                conn = open_connection();
            }
        } else if (!strcmp(action, "LOCAL")){
            close(conn);
            conn = 0;
        } else if (!strcmp(action, "DIR")){
            print_dir_files();
        }
    }

}


int main() {
    shell();
    return 0;
}