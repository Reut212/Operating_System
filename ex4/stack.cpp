# include <stdio.h>
# include <stdlib.h>
# include <string.h>

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

void TOP(){
    printf("%s", stk.top->data);
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

int main(){
    char data[1024] = "hello\n";
    PUSH(data);
    char data2[1024] = "hello2\n";
    PUSH(data2);
    char data3[1024] = "hello3\n";
    PUSH(data3);
    TOP();
    POP();
    TOP();
}