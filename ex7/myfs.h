#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_MAX 128
#define BLOCKSIZE 2
#define NAME_SIZE 9
#define MAX_DIR 10
#define BUFF_SIZE 80
#define FILES_MAX 10000


typedef struct superblock{
    int num_inodes;
    int num_blocks;
    int size_blocks;
}superblock;

typedef struct inode{
    int size;
    int first_block;
    char name[NAME_SIZE+1];
    int dir; //0 file 1 dir
}inode;

typedef struct disk_block{
    int next_block_num;
    char data[BLOCKSIZE];
}disk_block;

typedef struct myopenfile{
    int fd;
    int pos;
    inode file_inode;
    int current_block_index;
} myopenfile;

typedef struct mydirent {
    int size;
    int fds[MAX_DIR];
    char d_name[NAME_SIZE];
}mydirent;

typedef struct myDIR {
    int path;
}myDIR;

myopenfile open_f [FILES_MAX];
superblock sb;
inode *inodes;
disk_block *d_block;

// exercise's functions
int mymount(const char *source, const char *target,
            const char *filesystemtype, unsigned long
            mountflags, const void *data);
int myopen(const char *pathname, int flags);
int myclose(int myfd);
ssize_t myread(int myfd, void *buf, size_t count);
ssize_t mywrite(int myfd, const void *buf, size_t count);
off_t mylseek(int myfd, off_t offset, int whence);
myDIR *myopendir(const char *name);
struct mydirent *myreaddir(myDIR *dirp);


// other functions
void mymkfs(int fs_size);
int myclosedir(myDIR *dirp);
void mount_fs(const char *source);  // load adile system
void sync_fs(const char *target);   // write the file system
void print_fs(); // print out info about file system
void set_filesize(int filenum, int size);
void write_data(int filenum, int _pos, char data);
char read_data(int filenum, int pos);
int create_file(); // retrun file discriptor
struct mydirent *myread_dir(int fd);
char* read_data_test(int filenum, int pos);
int Myopen_dir(const char *pathname);
int find_empty_block();
int open_index(int myfd);
int find_empty_openfile();