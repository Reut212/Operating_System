
#include <stdio.h>

#ifndef EX7_MYLIBC_H
#define EX7_MYLIBC_H

#endif //EX7_MYLIBC_H

typedef struct myFILE
{
    int file_ptr;
    char* data;
    int size;
    char modes[2]; // 0 -> for letter, 1 -> for sign.
    int pos;
}myFILE;

myFILE *myfopen(const char *restrict pathname, const char *mode);
int myfclose(myFILE *stream);
size_t myfread(void *restrict ptr, size_t size, size_t nmemb, myFILE *stream);
size_t myfwrite(const void *restrict ptr, size_t size, size_t nmemb, myFILE *stream);
int myfseek(myFILE *stream, long offset, int whence);
int myfscanf(myFILE *restrict stream, const char *restrict format, ...);
int myfprintf(myFILE *restrict stream, const char *restrict format, ...);