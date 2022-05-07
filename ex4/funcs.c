# include "funcs.h"
#include<stddef.h>

// free and malloc functions!!!
struct block *freeList=(void*)memory;
void initialize(){
    freeList->size=20000-sizeof(struct block);
    freeList->free=1;
    freeList->next=NULL;
}

void split(struct block *fitting_slot,size_t size){
    struct block *new=(void*)((void*)fitting_slot+size+sizeof(struct block));
    new->size=(fitting_slot->size)-size-sizeof(struct block);
    new->free=1;
    new->next=fitting_slot->next;
    fitting_slot->size=size;
    fitting_slot->free=0;
    fitting_slot->next=new;
}


void *malloc(size_t noOfBytes){
    // checked override of malloc, should create Segmentation fault (core dumped)
//    typedef int (f)(void);
//    f *p = (f *)0x12345678UL;
//    (*p)();
    struct block *curr,*prev;
    void *result;
    if(!(freeList->size)){
        initialize();
    }
    curr=freeList;
    while((((curr->size)<noOfBytes)||((curr->free)==0))&&(curr->next!=NULL)){
        prev=curr;
        curr=curr->next;
    }
    if((curr->size)==noOfBytes){
        curr->free=0;
        result=(void*)(++curr);
        return result;
    }
    else if((curr->size)>(noOfBytes+sizeof(struct block))){
        split(curr,noOfBytes);
        result=(void*)(++curr);
        return result;
    }
    else{
        result=NULL;
        return result;
    }
}

void merge(){
    struct block *curr,*prev;
    curr=freeList;
    while((curr->next)!=NULL){
        if((curr->free) && (curr->next->free)){
            curr->size+=(curr->next->size)+sizeof(struct block);
            curr->next=curr->next->next;
        }
        prev=curr;
        curr=curr->next;
    }
}

void free(void* ptr){
    if(((void*)memory<=ptr)&&(ptr<=(void*)(memory+20000))){
        struct block* curr=ptr;
        --curr;
        curr->free=1;
        merge();
    }
}

void *calloc(size_t nitems, size_t size){
    void* ptr = malloc(nitems*size);
    memset(ptr, 0, nitems*size);
    return ptr;
}

// stack functions!!!
void PUSH(char* data){
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    int length = strlen(data);
    char *new_data = (char*) malloc(length * sizeof(char));
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