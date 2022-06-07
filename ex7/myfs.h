#include <aio.h>

# define FILES_MAX 10000

struct general_info {
    int num_inodes;
    int num_blocks;
    int size_blocks;
}ginfo;

struct inode {
    int size;
    int first_block;
    char name[8];
}*inodes;

struct disk_info {
    int next_block_num;
    char data[512];
}*dinfo;

typedef struct myDIR{

}myDIR;

typedef struct mydirent{
    char* name_d;
}mydirent;

typedef struct myopenfile{

}myopenfile;

void mymkfs(char fs_size);
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
int myclosedir(myDIR *dirp);