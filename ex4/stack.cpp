# include "stack.hpp"

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
    int length = strlen(data);
    char *new_data = (char*) malloc(length * sizeof(char));
    strcpy(new_data,data);
    new_node->data_ptr=new_data;
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
    if (stk.head==NULL){
        return(strdup(""));
    }
//    printf("%s", stk.top->data_ptr);
    return(stk.top->data_ptr);
}

void POP(){
    if (stk.head!=NULL){
        node* curr = stk.head;
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
            node* old_top = stk.top;
            curr->next=NULL;
            stk.top=curr;
            free(old_top->data_ptr);
            free(old_top);
        }

    }
}