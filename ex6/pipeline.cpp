#include <cstdio>
#include "active_object.hpp"
#include "Queue.hpp"

char caesar_cipher(char c) {
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

char small_big_letters(char c) {
    if ('A' <= c && c <= 'Z') {
        return (char) ((int) c + 'a' - 'A');
    } else if ('a' <= c && c <= 'z') {
        return (char) ((int) c - 'a' + 'A');
    } else {
        perror("not a valid char");
        return '!';
    }
}

int main() {
    void* q = createQ();
    enQ(q, (void*) 'a');
    enQ(q, (void*) 'V');
    enQ(q, (void*) 'z');
    enQ(q, (void*) 'Z');
    ao active_obj = newAO((void*) q, (beforeFun)caesar_cipher, (afterFun)small_big_letters);
    printQ(active_obj.q);
}