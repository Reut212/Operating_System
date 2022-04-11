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
void PUSH(char data[1024]);
void TOP();
void POP();