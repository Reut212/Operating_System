# include <iostream>
# include "stack.hpp"

struct node{
    node* next;
    char* data;
};

struct stack{
    node* head = NULL;
    node* top;
} stk;

void PUSH(char data[1024]){
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->data=data;
    new_node->next=NULL;
    if (stk.head== NULL){
        stk.head=new_node;
    }
    else{
        node* curr= stk.head;
        while (curr->next!=NULL){
            curr=curr->next;
        }
        curr->next=new_node;
    }
    stk.top=new_node;
}

char* TOP(){
    printf("%s", stk.top->data);
    return(stk.top->data);
}

void POP(){
    if (stk.head!=NULL){
        node* curr = stk.head;
        while (curr->next!=stk.top){
            curr = curr->next;
        }
        node* old_top = stk.top;
        curr->next=NULL;
        stk.top=curr;
        free(old_top);
    }
}
