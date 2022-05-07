#include <stdio.h>
#include <string.h>

int main()
{
    FILE *server, *client;
    char buffer[50];
    sprintf(buffer,"name first");
    client=popen("./client","w"); // wc -c -> is the process which counts the number of characters passed. 2nd parameter is "w" which means pipe is opened in writing mode
    server=popen("./server","r"); // wc -c -> is the process which counts the number of characters passed. 2nd parameter is "w" which means pipe is opened in writing mode
    fwrite(buffer,sizeof(char),strlen(buffer),client); // to write the data into the pipe
    pclose(server);
    pclose(client);
}