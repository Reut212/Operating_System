# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#include<stdio.h>
#include<stddef.h>
#include <sys/mman.h>
#pragma once
// malloc free funcs

struct block{
    size_t size;
    int is_available;
    struct block *next_meta_data;
};

void initialize();
void split(struct block *old, size_t size);
void *malloc(size_t number_of_bytes);
void merge();
void free(void* ptr);
void *calloc(size_t nitems, size_t size);

// stack functions!!!

#define MAXDATASIZE 1024

typedef struct Stack{
    int counter;
    char data[MAXDATASIZE];
} stack;

void PUSH(stack* stk, char *data);
char *TOP(stack *stk);
void POP(stack *stk);