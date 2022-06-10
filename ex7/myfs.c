#include <string.h>
# include "myfs.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define BLOCKSIZE 512

void mymkfs(int fs_size) {

    int size = fs_size - (int)sizeof(sb);
    sb.num_inodes = (int)((size*0.1)/ sizeof(inode));
    sb.num_blocks = (int)((size*0.9) / sizeof(disk_block));
    sb.size_blocks = (int)((size*0.9));
    inodes = malloc (sizeof (struct inode) * sb.num_inodes);
    for(int i = 0; i<sb.num_inodes; i++) {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy (inodes[i].name, "");
    }

    d_block = malloc (sizeof (disk_block) * sb.size_blocks);
    for (int i=0; i<sb.num_blocks; i++) {
        d_block[i].next_block_num = -1;
    }

    for (int i=0; i<FILES_MAX; i++){
        myopenfile* openfile =malloc (sizeof (myopenfile));
        openfile->fd=-1;
        open_f[i] = *openfile;
    }
}

int mymount(const char *source, const char *target,
            const char *filesystemtype, unsigned long
            mountflags, const void *data){
    if (source == NULL || target == NULL){
        perror("No source or target");
        return -1;
    }
    FILE *file = fopen(source, "r");
    // super block
    fread(&sb, sizeof(superblock), 1, file);

    // inodes
    inodes = malloc(sizeof(inode) * sb.num_inodes);
    fread(inodes, sizeof(struct inode), sb.num_inodes, file);

    // dbs
    d_block = malloc(sizeof(superblock) * sb.num_blocks);
    fread(d_block, sizeof(disk_block), sb.num_blocks, file);
    fclose(file);
    return 0;
}

void shorten_file(int bn)
{
    int nn = d_block[bn].next_block_num;
    if(d_block[bn].next_block_num >= 0)
    {
        shorten_file(nn);
    }
    d_block[bn].next_block_num = -1;
}
int find_empty_block()
{
    for (int i = 0; i < sb.num_blocks; i++)
    {
        if (d_block[i].next_block_num == -1)
        {
            return i;
        }
    }
    return -1;
}

void set_filesize(int filenum, int size)
{
    int temp = size + BLOCKSIZE -1;
    int num = temp / BLOCKSIZE;
    int bn = inodes[filenum].first_block;
    //grow the file if necessary
    for (num-- ; num > 0; num--)
    {
        //check next block number
        int next_num = d_block[bn].next_block_num;
        if (next_num == -2)
        {
            int empty = find_empty_block();
            d_block[bn].next_block_num = empty;
            d_block[empty].next_block_num = -2;
        }
        bn = d_block[bn].next_block_num;
    }
    //short the file if necessary
    shorten_file(bn);
    d_block[bn].next_block_num = -2;
}

int get_block_num(int file, int offeset)
{
    int bn = inodes[file].first_block;
    for(int togo = offeset; togo > 0; togo--)
    {
        bn = d_block[bn].next_block_num;
    }
    return bn;
}

int find_empty_openfile(){
    for (int i=0; i<FILES_MAX; i++){
        if(open_f[i].fd==-1){
            return i;
        }
    }
    return -1;
}

int myopen(const char *pathname, int flags){
    int index = find_empty_openfile();
    if (index==-1){
        perror("Too many files are open");
        return -1;
    }
    int file = open(pathname, flags, "r");
    open_f[index].fd = file;
    open_f[index].pos = 0;
    return file;
}

int open_index(int myfd){
    for (int i=0; i<FILENAME_MAX; i++){
        if (open_f[i].fd==myfd){
            return i;
        }
    }
    return -1;
}

int myclose(int myfd){
    int index = open_index(myfd);
    if (index == -1){
        perror("You are trying to close a file that is not open!");
        return -1;
    }
    open_f[index].fd=-1;
    open_f[index].pos=0;
    return 0;
}
