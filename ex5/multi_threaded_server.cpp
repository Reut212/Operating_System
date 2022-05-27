/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/mman.h>
#define PORT "3490"  // the port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold

struct node{
    struct node* next;
    char* data_ptr;
};
struct block{
    size_t size;
    int is_available;
    struct block *next_meta_data;
};
// free and malloc functions!!!
char *memory = (char *) mmap(NULL, sizeof(char) * 40000, PROT_READ | PROT_WRITE,
                             MAP_SHARED | MAP_ANONYMOUS, -1, 0);
struct block *mata_data_list=(struct block*)memory;
void initialize(){
    mata_data_list->size= 40000 - sizeof(struct block);
    mata_data_list->is_available=1;
    mata_data_list->next_meta_data=NULL;
}

void split(struct block *old, size_t size){
    struct block *new_block=(struct block*)((int*)old + size + sizeof(struct block));
    new_block->size= (old->size) - size - sizeof(struct block);
    new_block->is_available=1;
    new_block->next_meta_data=old->next_meta_data;
    old->size=size;
    old->is_available=0;
    old->next_meta_data=new_block;
}


void *malloc(size_t number_of_bytes){
    // checked override of malloc, should create Segmentation fault (core dumped)
//    typedef int (f)(void);
//    f *p = (f *)0x12345678UL;
//    (*p)();
    struct block *curr;
    void *result;
    if(!(mata_data_list->size)){
        initialize();
    }
    curr=mata_data_list;
    while((((curr->size) < number_of_bytes) || ((curr->is_available) == 0)) && (curr->next_meta_data != NULL)){
        curr=curr->next_meta_data;
    }
    if((curr->size) == number_of_bytes){
        curr->is_available=0;
        result=(void*)(++curr);
        return result;
    }
    else if((curr->size)>(number_of_bytes + sizeof(struct block))){
        split(curr, number_of_bytes);
        result=(void*)(++curr);
        return result;
    }
    else{
        result=NULL;
        return result;
    }
}

void merge(){
    struct block *curr;
    curr=mata_data_list;
    while((curr->next_meta_data) != NULL){
        if((curr->is_available) && (curr->next_meta_data->is_available)){
            curr->size+= (curr->next_meta_data->size) + sizeof(struct block);
            curr->next_meta_data=curr->next_meta_data->next_meta_data;
        }
        curr=curr->next_meta_data;
    }
}

void free(void* ptr) throw(){
    if(((void*)memory<=ptr)&&(ptr<=(void*)(memory+40000))){
        struct block* curr=(struct block*)ptr;
        --curr;
        curr->is_available=1;
        merge();
    }
}

void *calloc(size_t nitems, size_t size){
    void* ptr = malloc(nitems*size);
    memset(ptr, 0, nitems*size);
    return ptr;
}// stack functions!!!
struct funcs{
    struct node* head;
    struct node* top;
} stk;

void PUSH(char* data){
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    int length = strlen(data);
    char *new_data = (char*) malloc(length * sizeof(char)+1);
    strcpy(new_data,data);
    new_node->data_ptr=new_data;
    new_node->next=NULL;
    if (stk.head== NULL){
        stk.head=new_node;
    }
    else{
        struct node* curr= stk.head;
        while (curr->next!=NULL){
            curr=curr->next;
        }
        curr->next=new_node;
    }
    stk.top=new_node;
}

char* TOP(){
    if (stk.head==NULL){
        return(strdup(""));
    }
//    printf("%s", stk.top->data_ptr);
    return(stk.top->data_ptr);
}

void POP(){
    if (stk.head!=NULL){
        struct node* curr = stk.head;
        if (stk.head==stk.top){
            free(stk.top->data_ptr);
            free(stk.top);
            stk.top=NULL;
            stk.head=NULL;
        }
        else{
            while (curr->next!=stk.top){
                curr = curr->next;
            }
            struct node* old_top = stk.top;
            curr->next=NULL;
            stk.top=curr;
            free(old_top->data_ptr);
            free(old_top);
        }
    }
}

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void handle_stack(int self, int newSocket) {
    struct flock lock;
    memset (&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;

    printf("------------------\n");
    printf("client %d connected\n", self);
    char buf[6];
    while (1) {
        memset(buf, 0, 6);
        if ((recv(newSocket, buf, 6, 0)) == -1)
            perror("recv");
        if (strcmp(buf, "PUSH") == 0) {
            char data_push[1024];
            if ((recv(newSocket, data_push, 1024, 0)) == -1)
                perror("recv");
            else{
                fcntl (newSocket, F_SETLKW, &lock);
                PUSH(data_push);
                printf("client %d pushed %s to the stack\n", self, data_push);
                fcntl (newSocket, F_SETLKW, &lock);
            }
        } else if (strcmp(buf, "POP") == 0) {
            fcntl (newSocket, F_SETLKW, &lock);(&lock);
            POP();
            printf("popped from stack\n");
            fcntl (newSocket, F_SETLKW, &lock);

        } else if (strcmp(buf, "TOP") == 0) {
            fcntl (newSocket, F_SETLKW, &lock);
            char* top = TOP();
            if (send(newSocket, top, 1024, 0) == -1)
                perror("send");
            fcntl (newSocket, F_SETLKW, &lock);
        }
        else if (strcmp(buf, "EXIT") == 0) {
            printf("client %d disconnected\n", self);
            close(newSocket);
            exit(0);
        }
    }
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");
    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);


        if (!fork()) { // this is the child process
            int pid = getpid();
            handle_stack(pid, new_fd);
        }
    }

    return 0;
}