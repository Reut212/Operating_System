#include <cstdio>
#include "active_object.hpp"
#include "Queue.hpp"

typedef struct pipeline{
    ao ao1;
    ao ao2;
    ao ao3;
} pipeline;

char caesar_cipher(char c);
char small_big_letters(char c);