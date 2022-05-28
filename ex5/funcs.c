# include "funcs.h"

// free and malloc functions!!!
char *memory = NULL;
struct block *mata_data_list = NULL;
void initialize() {
    memory = (char *) mmap(NULL, sizeof(char) * 40000, PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    mata_data_list = (struct block *) memory;
    mata_data_list->size = 40000 - sizeof(struct block);
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


void *malloc(size_t number_of_bytes) {
    // checked override of malloc, should create Segmentation fault (core dumped)
//    typedef int (f)(void);
//    f *p = (f *)0x12345678UL;
//    (*p)();
    struct block *curr;
    void *result;
    if (memory==NULL) {
        initialize();
    }
    curr = mata_data_list;
    while ((((curr->size) < number_of_bytes) || ((curr->is_available) == 0)) && (curr->next_meta_data != NULL)) {
        curr = curr->next_meta_data;
    }
    if ((curr->size) == number_of_bytes) {
        curr->is_available = 0;
        result = (void *) (++curr);
        return result;
    } else if ((curr->size) > (number_of_bytes + sizeof(struct block))) {
        split(curr, number_of_bytes);
        result = (void *) (++curr);
        return result;
    } else {
        result = NULL;
        return result;
    }
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

void free(void *ptr) {
    if (((void *) memory <= ptr) && (ptr <= (void *) (memory + 40000))) {
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

void PUSH(stack *stk, char *data) {
    for (int i = 0; i < strlen(data); i++) {
        stk->data[stk->counter] = data[i];
        stk->counter++;
    }
    stk->data[stk->counter++] = '\0';
}

char *TOP(stack *stk) {
    if (stk->counter == 0) {
        perror("stack is empty");
    }
    char *res = (char *) malloc(sizeof(char) * 1024);
    // memory looks like this: ___/0____/0
    // we are now pointing to the cell after the last /0
    // so we want to go 2 cells back to point to the last char
    int x = (stk->counter) - 2;
    // now we'll seek for the separating /0
    while (stk->data[x] != '\0') {
        x--;
    }
    x++;
    strcpy(res, &(stk->data[x]));
    return res;
}

void POP(stack *stk) {
    // note: freeing space that had been malloc at the end when stack is destroyed
    // munmap at the end of the server
    while (stk->data[stk->counter - 2] != '\0') {
        stk->counter--;
    }
    stk->counter--;
}