#include<stdio.h>
#include<stddef.h>

#pragma once

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
