#include<stddef.h>
# include "funcs.hpp"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

// free and malloc functions!!!
char *memory = (char *) mmap(NULL, sizeof(char) * 10000000, PROT_READ | PROT_WRITE,
                             MAP_SHARED | MAP_ANONYMOUS, -1, 0);
struct block *mata_data_list = (struct block *) memory;

void initialize() {
    mata_data_list->size = 10000000 - sizeof(struct block);
    mata_data_list->is_available = 1;
    mata_data_list->next_meta_data = NULL;
}

void split(struct block *old, size_t size) {
    struct block *new_block = (struct block *) ((int *) old + size + sizeof(struct block));
    new_block->size = (old->size) - size - sizeof(struct block);
    new_block->is_available = 1;
    new_block->next_meta_data = old->next_meta_data;
    old->size = size;
    old->is_available = 0;
    old->next_meta_data = new_block;
}
#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define CHUNK_SIZE (1 << 14)
#define CHUNK_ALIGN(size) (((size)+(CHUNK_SIZE-1)) & ~(CHUNK_SIZE-1))

void *current_avail = NULL;
size_t current_avail_size = 0;

void *malloc(size_t size) {
    size_t newsize = ALIGN(size);
    void *p;
    if (current_avail_size < newsize) {
        current_avail = mmap(0, CHUNK_ALIGN(newsize),
                             PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,
                             -1, 0);
        current_avail_size = CHUNK_ALIGN(newsize);
    }
    p = current_avail;
    current_avail = current_avail+ newsize;
    current_avail_size -= newsize;
    return p;
}

void merge() {
    struct block *curr;
    curr = mata_data_list;
    while ((curr->next_meta_data) != NULL) {
        if ((curr->is_available) && (curr->next_meta_data->is_available)) {
            curr->size += (curr->next_meta_data->size) + sizeof(struct block);
            curr->next_meta_data = curr->next_meta_data->next_meta_data;
        }
        curr = curr->next_meta_data;
    }
}

void free(void *ptr) noexcept {
    if (((void *) memory <= ptr) && (ptr <= (void *) (memory + 10000000))) {
        struct block *curr = (struct block *) ptr;
        --curr;
        curr->is_available = 1;
        merge();
    }
}

void *calloc(size_t nitems, size_t size) {
    void *ptr = malloc(nitems * size);
    memset(ptr, 0, nitems * size);
    return ptr;
}

// stack functions!!!


void PUSH(struct funcs *stk, char *data) {
    struct node *new_node = (struct node *) malloc(sizeof(struct node));
    int length = strlen(data);
    char *new_data = (char *) malloc(length * sizeof(char) + 1);
    strcpy(new_data, data);
    new_node->data_ptr = new_data;
    new_node->next = NULL;
    if (stk->head == NULL) {
        stk->head = new_node;
    } else {
        struct node *curr = stk->head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_node;
    }
    stk->top = new_node;
}

char *TOP(struct funcs *stk) {
    if (stk->head == NULL) {
        return (strdup(""));
    }
//    printf("%s", stk.top->data_ptr);
    return (stk->top->data_ptr);
}

void POP(struct funcs *stk) {
    if (stk->head != NULL) {
        struct node *curr = stk->head;
        if (stk->head == stk->top) {
            free(stk->top->data_ptr);
            free(stk->top);
            stk->top = NULL;
            stk->head = NULL;
        } else {
            while (curr->next != stk->top) {
                curr = curr->next;
            }
            struct node *old_top = stk->top;
            curr->next = NULL;
            stk->top = curr;
            free(old_top->data_ptr);
            free(old_top);
        }
    }
}