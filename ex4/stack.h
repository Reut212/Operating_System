# include <stdio.h>
# include <stdlib.h>
# include <string.h>

#pragma once

struct node{
    struct node* next;
    char* data_ptr;
};

struct stack{
    struct node* head;
    struct node* top;
} stk;

void PUSH(char* data);
char* TOP();
void POP();
