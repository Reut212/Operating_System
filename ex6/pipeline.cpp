# include "pipline.hpp"
#include "server.cpp"

void* queue1 = q;
void* queue2 = createQ();
void* queue3 = createQ();

void enQ_to_queue2(void* n){
    enQ(queue2,n);
}

void enQ_to_queue3(void* n){
    enQ(queue3,n);
}

pipeline* create_pipeline(){
    pipeline *p = (pipeline *) malloc(sizeof(pipeline));
    p->ao1 = newAO(queue1, (beforeFun)caesar_cipher, (afterFun)enQ_to_queue2);
    p->ao1 = newAO(queue1, (beforeFun)small_big_letters, (afterFun)enQ_to_queue3);
    return p;
}

char cesare_cipher_char(char c){
    if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) {
        if (c == 'z') {
            return 'a';
        } else if (c == 'Z') {
            return 'A';
        } else {
            return (char) ((int) c + 1);
        }
    } else {
        perror("not a valid char");
        return '!';
    }
}

void* caesar_cipher(void* s) {
    char* str = (char*)s;
    for (int i=0; i<strlen(str); i++){
        str[i] = cesare_cipher_char(str[i]);
    }
    return (void*)str;
}

char small_big_letters_char(char c) {
    if ('A' <= c && c <= 'Z') {
        return (char) ((int) c + 'a' - 'A');
    } else if ('a' <= c && c <= 'z') {
        return (char) ((int) c - 'a' + 'A');
    } else {
        perror("not a valid char");
        return '!';
    }
}

void* small_big_letters(void* s) {
    char* str = (char*)s;
    for (int i=0; i<strlen(str); i++){
        str[i] = small_big_letters_char(str[i]);
    }
    return (void*)str;
}

int main() {
    create_pipeline();

}