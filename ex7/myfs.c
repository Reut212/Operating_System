#include <string.h>
#include "myfs.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define BLOCKSIZE 512

void mymkfs(int fs_size) {
    int size = fs_size - (int) sizeof(sb);
    sb.num_inodes = (int) ((size * 0.1) / sizeof(inode));
    sb.num_blocks = (int) ((size * 0.9) / sizeof(disk_block));
    sb.size_blocks = (int) ((size * 0.9));
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    for (int i = 0; i < sb.num_inodes; i++) {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name, "");
    }

    d_block = malloc(sizeof(disk_block) * sb.size_blocks);
    for (int i = 0; i < sb.num_blocks; i++) {
        d_block[i].next_block_num = -1;
    }

    for (int i = 0; i < FILES_MAX; i++) {
        myopenfile *openfile = malloc(sizeof(myopenfile));
        openfile->fd = -1;
        open_f[i] = *openfile;
    }

    FILE *file;
    file = fopen("filesystem.txt", "w");

    fwrite(&sb, sizeof(struct superblock), 1, file);
    fwrite(inodes, sizeof(inode), sb.num_inodes, file);
    fwrite(d_block, sizeof(disk_block), sb.num_blocks, file);
    fclose(file);
}



int mymount(const char *source, const char *target,
            const char *filesystemtype, unsigned long
            mountflags, const void *data) {
    FILE* src = fopen(source, "r");

    //reading source data
    superblock* src_sb = malloc(sizeof(superblock));
    fread(src_sb, sizeof(superblock), 1, src);
    int src_num_block = src_sb->num_blocks;
    int src_size_blocks = src_sb->size_blocks;
    int src_num_inodes = src_sb->num_inodes;

    inode *src_inodes = malloc(sizeof(inode)*src_num_inodes);
    fread(src_inodes, sizeof(inode), 1, src);

    disk_block *src_dblock= malloc(sizeof(disk_block)*src_num_block);
    fread(src_dblock, sizeof(struct disk_block), 1, src);

    //updating target's data
    FILE* trg = fopen(target, "w");

    //update of super block
    superblock* trg_sb = malloc(sizeof(superblock));
    trg_sb->num_blocks= src_num_block;
    trg_sb->size_blocks=src_size_blocks;
    trg_sb->num_inodes=src_num_inodes;
    fwrite(&trg_sb, sizeof(superblock), 1, trg);

    //update of inodes
    inode *trg_inodes = malloc(sizeof(inode)*src_num_inodes);
    for (int i=0; i<src_num_inodes;i++){
        trg_inodes[i].size=src_inodes[i].size;
        trg_inodes[i].dir=src_inodes[i].dir;
        trg_inodes[i].first_block=src_inodes[i].first_block;
        strcpy(trg_inodes[i].name, src_inodes[i].name);
    }
    fwrite(&trg_inodes, sizeof(inode), 1, trg);

    //update of disk blocks
    disk_block *trg_dblock = malloc(sizeof(disk_block)*src_num_block);
    for (int i=0; i<src_num_block;i++){
        trg_dblock[i].next_block_num=src_dblock[i].next_block_num;
        strcpy(trg_dblock[i].data, src_dblock[i].data);
    }
    fwrite(&trg_dblock, sizeof(struct disk_block), 1, trg);

    fclose(trg);
    return 0;
}

void shorten_file(int bn) {
    int nn = d_block[bn].next_block_num;
    if (d_block[bn].next_block_num >= 0) {
        shorten_file(nn);
    }
    d_block[bn].next_block_num = -1;
}

int find_empty_block() {
    for (int i = 0; i < sb.num_blocks; i++) {
        if (d_block[i].next_block_num == -1) {
            return i;
        }
    }
    return -1;
}

void set_filesize(int filenum, int size) {
    int temp = size + BLOCKSIZE - 1;
    int num = temp / BLOCKSIZE;
    int bn = inodes[filenum].first_block;
    //grow the file if necessary
    for (num--; num > 0; num--) {
        //check next block number
        int next_num = d_block[bn].next_block_num;
        if (next_num == -2) {
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

int get_block_num(int file, int offeset) {
    int bn = inodes[file].first_block;
    for (int togo = offeset; togo > 0; togo--) {
        bn = d_block[bn].next_block_num;
    }
    return bn;
}

int find_empty_openfile() {
    for (int i = 0; i < FILES_MAX; i++) {
        if (open_f[i].fd == -1) {
            return i;
        }
    }
    return -1;
}

int myopen(const char *pathname, int flags) {
    int index = find_empty_openfile();
    if (index == -1) {
        perror("Too many files are open");
        return -1;
    }
    int file = open(pathname, flags, "r");
    open_f[index].fd = file;
    open_f[index].pos = 0;
    return file;
}

int open_index(int myfd) {
    for (int i = 0; i < FILENAME_MAX; i++) {
        if (open_f[i].fd == myfd) {
            return i;
        }
    }
    return -1;
}

int myclose(int myfd) {
    int index = open_index(myfd);
    if (index == -1) {
        perror("You are trying to close a file that is not open!");
        return -1;
    }
    open_f[index].fd = -1;
    open_f[index].pos = 0;
    return 0;
}

int main() {
    mymkfs(10000);
//    mymount(NULL, "fs_data", NULL, 0, NULL);
}