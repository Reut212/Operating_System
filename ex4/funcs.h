# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#include<stdio.h>
#include<stddef.h>
#pragma once
// stack funcs
struct node{
    struct node* next;
    char* data_ptr;
};

struct funcs{
    struct node* head;
    struct node* top;
} stk;

void PUSH(char* data);
char* TOP();
void POP();

// malloc free funcs
char memory[20000];

struct block{
    size_t size;
    int free;
    struct block *next;
};

void initialize();
void split(struct block *fitting_slot,size_t size);
void *malloc(size_t noOfBytes);
void merge();
void free(void* ptr);
void *calloc(size_t nitems, size_t size);

