#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define PATH_MAX 128
#define BLOCKSIZE 5
#define NAME_SIZE 9
#define MAX_DIR 10
#define BUFF_SIZE 80
#define FILES_MAX 10000


typedef struct superblock{
    int num_inodes;
    int num_blocks;
    int size_blocks;
    bool is_mounted;
}superblock;

typedef struct inode{
    int size; //number of blocks
    int first_block;
    char name[NAME_SIZE+1];
    char path[PATH_MAX+1];
    bool file; // true - file, false - dir
}inode;

typedef struct disk_block{
    int next_block_num;
    char data[BLOCKSIZE];
}disk_block;

typedef struct myopenfile{
    int fd;
    int file_inode;
    int current_block_index;
    int current_offset;
} myopenfile;

typedef struct mydirent {
    int size;
    int files_inode_indexes[MAX_DIR];
    char d_name[NAME_SIZE+1];
}mydirent;

typedef struct myDIR {
    int dir_inode_index;
}myDIR;

myopenfile open_f [FILES_MAX];
superblock sb;
inode *inodes;
disk_block *d_block;

// exercise's functions
void mymkfs(int fs_size);
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
int find_empty_inode();
int create_file(char *pathname, int flags, bool isfile, char *path);
int check_if_file_exist(char *filename, int flags, bool isfile, char *path);
int alloc_new_block(int last_block_index);
void myseek(int index, int offset, int how_much_bytes_to_move);
int where_we_are_now(int index);
int calc_end(int index);
int find_dir_inode(const char *name, const char *path);
void find_subfiles(mydirent *dirent, char *path);
int find_empty_block();
int open_index(int myfd);
int find_empty_openfile();
