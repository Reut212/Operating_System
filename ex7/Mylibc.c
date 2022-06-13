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
    file->size = inodes[id].size; //update file size

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

size_t myfwrite(void * ptr, size_t size, size_t nmemb, myFILE * stream){
    if(stream->modes[0] != 'w' && stream->modes[0] != 'a'
        && (stream->modes[0] != 'r' && stream->modes[1] != '+')){
        perror("Not the right mode for writing!");
        return -1;
    }
    size_t start_index = open_index(stream->file_ptr);
    size_t last_index = mywrite(stream->file_ptr, ptr, nmemb * size);
    return last_index - start_index;
}