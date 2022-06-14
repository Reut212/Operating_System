#include <stdarg.h>
#include "mystdio.h"
#include "myfs.h"

myFILE *myfopen(const char *pathname, const char *mode){
    myFILE *file = (myFILE *)malloc(sizeof(myFILE));
    int id = myopen(pathname, 0);
    if(strlen(mode) == 1){ //if one char accepted
        file->modes[0] = mode[0];
    }
    else if(strlen(mode) == 2){ //if chars accepted
        file->modes[0] = mode[0];
        file->modes[1] = mode[1];
    }
    else{
        perror("Not a valid mode!");
        return NULL;
    }
    if((strcmp(mode, "a") != 0 && strcmp(mode, "w") != 0 &&
        strcmp(mode, "r") != 0 && strcmp(mode, "r+") != 0)){ //if not a valid mode accepted
        perror("Not a valid mode!");
        file->modes[0] = ' '; //clean modes
        file->modes[1] = ' ';
        return NULL;
    }
    file->file_fd = id; //update id of file

    if(!strcmp(mode, "a")){ //if mode is "append"
        mylseek(id, 0, SEEK_END);
    }
    else if(!strcmp(mode, "w")){ //if mode is "write"
        inodes[id].size = 0;
    }
    file->offset=0;
    return file;
}

int myfclose(myFILE *stream){
    if (stream == NULL){
        perror("Null pointer exception");
        return -1;
    }
    if(myclose(stream->file_fd) == 0){
        free(stream);
        return 0;
    }
    free(stream);
    return -1;
}

size_t myfread(void * ptr, size_t size, size_t nmemb, myFILE * stream){
    if (stream == NULL){
        perror("Null pointer exception");
        return -1;
    }
    if(stream->modes[0] == 'w' || stream->modes[0] == 'a'){
        perror("Not the right mode for reading!");
        return -1;
    }
    mylseek(stream->file_fd,stream->offset,SEEK_SET);
    size_t bytes_read = myread(stream->file_fd, ptr, nmemb * size);
    return bytes_read;
}

size_t myfwrite(const void *ptr, size_t size, size_t nmemb, myFILE *stream){
    if (stream == NULL){
        perror("Null pointer exception");
        return -1;
    }
    if(stream->modes[0] == 'r' && stream->modes[1]!='+'){
        perror("Not the right mode for writing!");
        return -1;
    }
    mylseek(stream->file_fd,stream->offset,SEEK_SET);
    size_t bytes_written = mywrite(stream->file_fd, ptr, nmemb * size);
    return bytes_written;
}

int myfseek(myFILE *stream, long offset, int whence){
    if (stream == NULL){
        perror("Null pointer exception");
        return -1;
    }
    if(whence == SEEK_SET){
        stream->offset = mylseek(stream->file_fd, offset, SEEK_SET);
    }else if(whence == SEEK_CUR){
        stream->offset = mylseek(stream->file_fd, offset, SEEK_CUR);
    }else if (whence == SEEK_END){
        stream->offset = mylseek(stream->file_fd, offset, SEEK_END);
    }
    return stream->offset;
}

int myfscanf(myFILE  *stream, const char *format, ...){
    if (stream == NULL){
        perror("Null pointer exception");
        return -1;
    }
    int format_size = (int)strlen(format);
    va_list arguments;
    float* fl=(float*)va_arg(arguments, void *);
    char* ch=(char*)va_arg(arguments, void *);
    int* dig=(int *)va_arg(arguments, void *);
    int i;
    /* Initializing arguments to store all values after num */
    va_start ( arguments, format );
    for ( i = 0; i < format_size; i++ ){
        if(format[i] == '%'){
            if (format[i+1] == 'f'){
                myfread(fl, sizeof(float), 1, stream);
            }
            if (format[i+1] == 'c'){
                myfread(ch, sizeof(char), 1, stream);
            }
            if (format[i+1] == 'd'){
                myfread(dig, sizeof(int), 1, stream);
            }
        }
    }
    return i;
}

int myfprintf(myFILE  *stream, const char *format, ...){
    if (stream == NULL){
        perror("Null pointer exception");
        return -1;
    }
    int format_size = (int)strlen(format);
    va_list arguments;
    float* fl=(float*)va_arg(arguments, void *);
    char* ch=(char*)va_arg(arguments, void *);
    int* dig=(int *)va_arg(arguments, void *);
    int counter = 0;
    /* Initializing arguments to store all values after num */
    va_start ( arguments, format );
    for ( int i = 0; i < format_size; i++ ){
        if(format[i] == '%'){
            if (format[i+1] == 'f'){
                myfwrite(fl, sizeof(float), 1, stream);
                counter++;
            }
            if (format[i+1] == 'c'){
                myfwrite(ch, sizeof(char), 1, stream);
                counter++;
            }
            if (format[i+1] == 'd'){
                myfwrite(dig, sizeof(int), 1, stream);
                counter++;
            }
        }
    }
    return counter;
}