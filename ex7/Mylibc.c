#include "Mylibc.h"
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
        return nullptr;
    }
    if((strcmp(mode, "a") != 0 && strcmp(mode, "w") != 0 &&
        strcmp(mode, "r") != 0 && strcmp(mode, "r+") != 0)){ //if not a valid mode accepted
        perror("Not a valid mode!");
        file->modes[0] = ' '; //clean modes
        file->modes[1] = ' ';
        return nullptr;
    }
    file->file_ptr = id; //update id of file
//    file->size = inodes[id].size; //update file size

    if(!strcmp(mode, "a")){ //if mode is "append"
        mylseek(id, 0, SEEK_END);
    }
    else if(!strcmp(mode, "w")){ //if mode is "write"
        inodes[id].size = 0;
    }
    return file;
}

int myfclose(myFILE *stream){
    if(myclose(stream->file_ptr) == 0){
    free(stream);
        return 0;
    }
    free(stream);
    return -1;
}

size_t myfread(void * ptr, size_t size, size_t nmemb, myFILE * stream){
    if(stream->modes[0] != 'r' || (stream->modes[0] != 'r' && stream->modes[1] != '+')){
        perror("Not the right mode for reading!");
        return -1;
    }
    size_t start_index = open_index(stream->file_ptr);
    size_t last_index = myread(stream->file_ptr, ptr, nmemb * size);
    return start_index - last_index;
}

size_t myfwrite(const void *ptr, size_t size, size_t nmemb, myFILE *stream){
    if(stream->modes[0] != 'w' && stream->modes[0] != 'a'
        && (stream->modes[0] != 'r' && stream->modes[1] != '+')){
        perror("Not the right mode for writing!");
        return -1;
    }
    size_t start_index = open_index(stream->file_ptr);
    size_t last_index = mywrite(stream->file_ptr, ptr, nmemb * size);
    return last_index - start_index;
}

int myfseek(myFILE *stream, long offset, int whence){
    if(whence == SEEK_SET){
        stream->pos = mylseek(stream->file_ptr, offset, SEEK_SET);
    }else if(whence == SEEK_CUR){
        stream->pos = mylseek(stream->file_ptr, offset, SEEK_CUR);
    }else if (whence == SEEK_END){
        stream->pos = mylseek(stream->file_ptr, offset, SEEK_END);
    }
    return stream->pos;
}

int myfscanf(myFILE  *stream, const char *format, ...){
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