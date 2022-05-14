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


void PUSH(struct funcs* stk, char* data);
char* TOP(struct funcs* stk);
void POP(struct funcs* stk);

// malloc free funcs
struct funcs{
    struct node* head;
    struct node* top;
};

struct block{
    size_t size;
    int is_available;
    struct block *next_meta_data;
};

void initialize();
void split(struct block *old, size_t size);
void *malloc(size_t number_of_bytes);
void merge();
void free(void* ptr) noexcept;
void *calloc(size_t nitems, size_t size);

