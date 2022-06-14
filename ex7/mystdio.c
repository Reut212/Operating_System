#include <stdarg.h>
#include "mystdio.h"
#include "myfs.h"

myFILE *myfopen(const char *pathname, const char *mode) {
    myFILE *file = (myFILE *) malloc(sizeof(myFILE));
    int id = myopen(pathname, 0);
    if (id == -1) {
        perror("Problem opening file!");
        return NULL;
    }
    if (strlen(mode) == 1) { //if one char accepted
        file->modes[0] = mode[0];
    } else if (strlen(mode) == 2) { //if chars accepted
        file->modes[0] = mode[0];
        file->modes[1] = mode[1];
    } else {
        perror("Not a valid mode!");
        return NULL;
    }
    if ((strcmp(mode, "a") != 0 && strcmp(mode, "w") != 0 &&
         strcmp(mode, "r") != 0 && strcmp(mode, "r+") != 0)) { //if not a valid mode accepted
        perror("Not a valid mode!");
        file->modes[0] = ' '; //clean modes
        file->modes[1] = ' ';
        return NULL;
    }
    file->file_fd = id; //update id of file

    if (!strcmp(mode, "a")) { //if mode is "append"
        mylseek(id, 0, SEEK_END);
    } else if (!strcmp(mode, "w")) { //if mode is "write"
        inodes[id].size = 0;
    }
    file->offset = 0;
    return file;
}

int myfclose(myFILE *stream) {
    if (stream == NULL) {
        perror("Null pointer exception");
        return -1;
    }
    if (myclose(stream->file_fd) == 0) {
        free(stream);
        return 0;
    }
    free(stream);
    return -1;
}

size_t myfread(void *ptr, size_t size, size_t nmemb, myFILE *stream) {
    if (stream == NULL) {
        perror("Null pointer exception");
        return -1;
    }
    if (stream->modes[0] == 'w' || stream->modes[0] == 'a') {
        perror("Not the right mode for reading!");
        return -1;
    }
    mylseek(stream->file_fd, stream->offset, SEEK_SET);
    size_t bytes_read = myread(stream->file_fd, ptr, nmemb * size);
    stream->offset = stream->offset + bytes_read;
    return bytes_read;
}

size_t myfwrite(const void *ptr, size_t size, size_t nmemb, myFILE *stream) {
    if (stream == NULL) {
        perror("Null pointer exception");
        return -1;
    }
    if (stream->modes[0] == 'r' && stream->modes[1] != '+') {
        perror("Not the right mode for writing!");
        return -1;
    }
    if (stream->modes[0] == 'w' || (stream->modes[0] == 'r' && stream->modes[1] == '+')) {
        int curr_offset = mylseek(stream->file_fd, 0, SEEK_CUR);
        mylseek(stream->file_fd, stream->offset, SEEK_SET);
        size_t bytes_written = mywrite(stream->file_fd, ptr, nmemb * size);
        stream->offset = curr_offset + bytes_written;
        return bytes_written;
    }
    // append
    int curr_offset = mylseek(stream->file_fd, 0, SEEK_END);
    mylseek(stream->file_fd, curr_offset, SEEK_SET);
    size_t bytes_written = mywrite(stream->file_fd, ptr, nmemb * size);
    stream->offset = curr_offset + bytes_written;
    return bytes_written;

}

int myfseek(myFILE *stream, long offset, int whence) {
    if (stream == NULL) {
        perror("Null pointer exception");
        return -1;
    }
    if (whence == SEEK_SET) {
        stream->offset = mylseek(stream->file_fd, offset, SEEK_SET);
    } else if (whence == SEEK_CUR) {
        stream->offset = mylseek(stream->file_fd, offset, SEEK_CUR);
    } else if (whence == SEEK_END) {
        stream->offset = mylseek(stream->file_fd, offset, SEEK_END);
    }
    return stream->offset;
}

int myfprintf(myFILE *restrict stream, const char *restrict format, ...) {
    if (stream == NULL) {
        perror("Null pointer exception");
        return -1;
    }
    int i = 0;
    va_list arguments;
    va_start(arguments, format);
    int *sum = va_arg(arguments, void*);
    char *c = va_arg(arguments, void*);
    float *f = va_arg(arguments, void*);
    char *trash = va_arg(arguments, void*);
    for (i = 0; i < (int) strlen(format); i++) {
        if (format[i] == '%') {
            if (format[i + 1] == 'd') {
                sum = va_arg(arguments, void*);
                myfwrite(&sum, sizeof(int), 1, stream);
            }
            else if (format[i + 1] == 'f') {
                f = va_arg(arguments, void*);
                myfwrite(&f, sizeof(float), 1, stream);
            }
            else if (format[i + 1] == 'c') {
                c = va_arg(arguments, void*);
                myfwrite(&c, sizeof(char), 1, stream);
            }
        }
    }
    return i;
}

int myfscanf(myFILE *stream, const char *format, ...) {
    if (stream == NULL) {
        perror("Null pointer exception");
        return -1;
    }
    int i;
    va_list arguments;
    va_start(arguments, format);
    int *sum = va_arg(arguments, void*);
    char *c = va_arg(arguments, void*);
    float *f = va_arg(arguments, void*);
    for (i = 0; i < (int) strlen(format); i++) {
        if (format[i] == '%') {
            if (format[i + 1] == 'd') {
                sum = va_arg(arguments, void*);
                myfread(&sum, sizeof(int), 1, stream);
            }
            if (format[i + 1] == 'f') {
                f = va_arg(arguments, void*);
                myfread(&f, sizeof(float), 1, stream);
            }
            if (format[i + 1] == 'c') {
                c = va_arg(arguments, void*);
                myfread(&c, sizeof(char), 1, stream);
            }
        }
    }
    return i;
}