
# include "stack.h"

struct node{
    node* next;
    char* data_ptr;
};

struct stack{
    node* head = NULL;
    node* top;
} stk;

void PUSH(char* data){
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    char* index = strstr(data, "\n");
    int length = index - data;
    char* ptr = (char*) malloc(length * sizeof(char));
    new_node->data_ptr=ptr;
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
    printf("%s", stk.top->data_ptr);
    return(stk.top->data_ptr);
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
        free(old_top->data_ptr);
        free(old_top);
    }
}
