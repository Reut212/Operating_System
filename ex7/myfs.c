#include <string.h>
#include "myfs.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

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
    FILE *src = fopen(source, "r");

    //reading source data
    superblock *src_sb = malloc(sizeof(superblock));
    fread(src_sb, sizeof(superblock), 1, src);
    int src_num_block = src_sb->num_blocks;
    int src_size_blocks = src_sb->size_blocks;
    int src_num_inodes = src_sb->num_inodes;

    inode *src_inodes = malloc(sizeof(inode) * src_num_inodes);
    fread(src_inodes, sizeof(inode), 1, src);

    disk_block *src_dblock = malloc(sizeof(disk_block) * src_num_block);
    fread(src_dblock, sizeof(struct disk_block), 1, src);

    //updating target's data
    FILE *trg = fopen(target, "w");

    //update of super block
    superblock *trg_sb = malloc(sizeof(superblock));
    trg_sb->num_blocks = src_num_block;
    trg_sb->size_blocks = src_size_blocks;
    trg_sb->num_inodes = src_num_inodes;
    fwrite(&trg_sb, sizeof(superblock), 1, trg);

    //update of inodes
    inode *trg_inodes = malloc(sizeof(inode) * src_num_inodes);
    for (int i = 0; i < src_num_inodes; i++) {
        trg_inodes[i].size = src_inodes[i].size;
        trg_inodes[i].file = src_inodes[i].file;
        trg_inodes[i].first_block = src_inodes[i].first_block;
        strcpy(trg_inodes[i].name, src_inodes[i].name);
    }
    fwrite(&trg_inodes, sizeof(inode), 1, trg);

    //update of disk blocks
    disk_block *trg_dblock = malloc(sizeof(disk_block) * src_num_block);
    for (int i = 0; i < src_num_block; i++) {
        trg_dblock[i].next_block_num = src_dblock[i].next_block_num;
        strcpy(trg_dblock[i].data, src_dblock[i].data);
    }
    fwrite(&trg_dblock, sizeof(struct disk_block), 1, trg);

    fclose(trg);
    fclose(src);
    return 0;
}


int find_empty_openfile() {
    for (int i = 0; i < FILES_MAX; i++) {
        if (open_f[i].fd == -1) {
            return i;
        }
    }
    return -1;
}


int find_empty_block() {
    for (int i = 0; i < sb.num_blocks; i++) {
        if (d_block[i].next_block_num == -1) {
            return i;
        }
    }
    return -1;
}

int find_empty_inode() {
    for (int i = 0; i < sb.num_inodes; i++) {
        if (inodes[i].first_block == -1) {
            return i;
        }
    }
    return -1;
}

int create_file(char *pathname, int flags, bool isfile, char *path) {
    int index = find_empty_openfile();
    if (index == -1) {
        perror("Too many files are open");
        return -1;
    }
//    int file = open(pathname, flags, "r");
    open_f[index].current_offset = 0;

    // finding inode and blocks
    int inode = find_empty_inode();
    if (inode == -1) {
        perror("file cant be used");
        return -1;
    }

    int curr_block = find_empty_block();
    if (curr_block == -1) {
        perror("curr_block == -1");
        return -1;
    }
    open_f[index].fd = inode;
    inodes[inode].size = 1;
    inodes[inode].first_block = curr_block;
    inodes[inode].file = isfile;
    strcpy(inodes[inode].name, pathname);
    strcpy(inodes[inode].path, path);
    d_block[curr_block].next_block_num = -2;


    open_f[index].file_inode = inode;
    open_f[index].current_block_index = curr_block;
    open_f[index].current_offset = 0;
    return inode;
}

// for each check if file exist if not create
// check if open if not insert to open_f
int check_if_file_exist(char *filename, int flags, bool isfile, char *path) {
    int inode_index = -1;
    // check if exists
    for (int i = 0; i < sb.num_inodes; i++) {
        if (!strcmp(inodes[i].name, filename)) {
            if (!strcmp(inodes[i].path, path)) {
                inode_index = i;
                break;
            }
        }
    }
    // exist - need to check if open
    if (inode_index != -1) {
        for (int i = 0; i < FILES_MAX; i++) {
            if (open_f[i].file_inode == inode_index) { // open
                return open_f[i].fd;
            }
        }
        // exist and closed
        int open_index = find_empty_openfile();
        open_f[open_index].file_inode = inode_index;
        int file = open(filename, flags, "r");
        open_f[open_index].fd = file;
        open_f[open_index].current_offset = 0;
        open_f[open_index].current_block_index = inodes[inode_index].first_block;
        return file;
    }
        // doesn't exist - create and insert to open_f
    else {
        return create_file(filename, flags, isfile, path);
    }
}

// pathname example: /home/reut/CS.txt
int myopen(const char *pathname, int flags) {
    char str[BUFF_SIZE];
    strcpy(str, pathname);
    int len = strlen(pathname);
    char *curr_tok = strtok(str, "/");
    char *arr[len];
    int i = 0;
    while (curr_tok) {
        arr[i] = curr_tok;
        curr_tok = strtok(NULL, "/");
        i++;
    }
    char *path[i + 1];
    path[0] = "";
    for (int j = 0; j < i - 1; j++) {
        check_if_file_exist(arr[j], flags, false, path[j]);
        int newSize = strlen(path[j]) + strlen(arr[j]) + 1;
        char *newBuffer = (char *) malloc(newSize);
        strcpy(newBuffer, path[j]);
        strcat(newBuffer, "/");
        strcat(newBuffer, arr[j]);
        strcpy(path[j + 1], newBuffer);
        free(newBuffer);
    }
    return check_if_file_exist(arr[i - 1], flags, true, path[i - 1]);
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
    open_f[index].fd = -1;
    open_f[index].current_offset = 0;
    open_f[index].file_inode = -1;
    open_f[index].current_block_index = -1;
    return 0;
}

int alloc_new_block(int last_block_index) {
    int block_index = find_empty_block();
    if (block_index == -1) {
        perror("not enough space!");
        return -1;
    }
    d_block[last_block_index].next_block_num = block_index;
    d_block[block_index].next_block_num = -2;
    return block_index;
}

int pos(int index, int stop) { //find number of bytes until current block and current offset
    int counter = 0;
    int curr = inodes[open_f[index].file_inode].first_block;
    while (curr != stop) {
        curr = d_block[curr].next_block_num;
        counter++;
    }
    return BLOCKSIZE * counter + open_f[index].current_offset;
}

void myseek(int index, int offset, int how_much_bytes_to_move) {
    int curr = inodes[open_f[index].file_inode].first_block;
    for (int i = 0; i < how_much_bytes_to_move/BLOCKSIZE; i++) {
        if (curr == -2) {
            perror("Not you're memory");
        }
        curr = d_block[curr].next_block_num;
    }
    open_f[index].current_offset = how_much_bytes_to_move % BLOCKSIZE;
    open_f[index].current_block_index = curr;
}

int where_we_are_now(int index) {
    int curr_block = inodes[open_f[index].file_inode].first_block;
    int curr_offset = 0;
    int stop_block = open_f[index].current_block_index;
    int stop_offset = open_f[index].current_offset;
    int bytes = 0;
    while (1){
        if (curr_block==stop_block&&curr_offset==stop_offset){
            break;
        }
        curr_offset++;
        if (curr_offset==BLOCKSIZE) {
            curr_offset=0;
            curr_block = d_block[curr_block].next_block_num;
        }
        bytes++;
    }
    return bytes;
}

int calc_end(int index) {
    int curr_block = inodes[open_f[index].file_inode].first_block;
    int curr_offset = 0;
    int bytes = 0;
    while (1){
        if (d_block[curr_block].data[curr_offset]==0){
            break;
        }
        curr_offset++;
        if (curr_offset==BLOCKSIZE) {
            curr_offset=0;
            curr_block = d_block[curr_block].next_block_num;
        }
        bytes++;
    }
    return bytes;
}

off_t mylseek(int myfd, off_t offset, int whence) {
    int index = open_index(myfd);
    if (index == -1) {
        perror("You are trying to seek from a file that is not open!");
        return -1;
    }
    if (whence == SEEK_SET) { // setting offset to given offset
        int how_much_bytes_to_move = (int)offset;
        myseek(index, offset, how_much_bytes_to_move);
    } else if (whence == SEEK_CUR) {
        int where_we_are = where_we_are_now(index);
        myseek(index, offset, where_we_are+(int)offset);
    } else if (whence == SEEK_END) {
        int end = calc_end(index);
        myseek(index, offset, end+(int)offset);
    }
    if (open_f[myfd].current_offset < 0) {
        open_f[myfd].current_offset = 0;
    }
    return open_f[myfd].current_offset;
}

ssize_t myread(int myfd, void *buf, size_t count) {
    int index = open_index(myfd);
    if (index == -1) {
        perror("You are trying to read from a file that is not open!");
        return -1;
    }
    buf = (char *) buf;
    int bytes_read = 0;
    int curr_block = open_f[index].current_block_index; // find the block number
    int offset = open_f[index].current_offset;
    for (int i = 0; i < count; i++) {
        if (offset >= BLOCKSIZE) {
            int block_index = d_block[curr_block].next_block_num;
            if (block_index == -1) {
                perror("Core dump");
                return -1;
            }
            offset = 0;
            curr_block = block_index;
        }
        ((char *) buf)[i] = d_block[curr_block].data[offset];
        offset++;
        bytes_read++;
    }
    mylseek(open_f[index].fd, count, SEEK_CUR);
    return bytes_read;
}

ssize_t mywrite(int myfd, const void *buf, size_t count) {
    int index = open_index(myfd);
    if (index == -1) {
        perror("You are trying to write to a file that is not open!");
        return -1;
    }
    int curr_block = open_f[index].current_block_index; // find the block number
    int offset = open_f[index].current_offset;
    char *data = (char *) buf;
    int new_blocks_allocated = 0;
    for (int i = 0; i < count; i++) {
        if (offset >= BLOCKSIZE) {
            int block_index = alloc_new_block(curr_block);
            if (block_index == -1) {
                perror("not enough space!");
                return -1;
            }
            offset = 0;
            new_blocks_allocated++;
            curr_block = block_index;
        }
        d_block[curr_block].data[offset] = data[i];
        offset++;
    }

    if (offset >= BLOCKSIZE) { // if block is now full allocate new block and moving offset
        int block_index = alloc_new_block(curr_block);
        if (block_index == -1) {
            perror("not enough space!");
            return -1;
        }
        offset = 0;
        new_blocks_allocated++;
        curr_block = block_index;
    }
    open_f[index].current_block_index = curr_block;
    open_f[index].current_offset = offset;  // moving offset to new position
    inodes[open_f[index].file_inode].size += new_blocks_allocated;
    return 0;
}

//// returns index if a directory is open in open_f
//int exists_in_open_f(const char *name){
//    for (int i=0; i<FILES_MAX; i++){
//        if (!strcmp(inodes[i].name,name)){
//            return i;
//        }
//    }
//    return -1;
//}
//
//// returns an index to open_f of a dir and create it if doesn't exist
//int dir_index_in_open_f(const char *name){
//    int index = exists_in_open_f(name);
//    if (index!=-1){ // exists
//        return index;
//    }
//    // doesn't exist
//    index = find_empty_openfile();
//    if (index == -1){ // not enough space in open_f array
//        perror("not enough space in open_f array");
//    }
//    // insert data in chosen index
//
//}
//
//
//
//myDIR *myopendir(const char *name) {
//    int index = dir_index_in_open_f(name);
//    myDIR *dir = (myDIR*)malloc(sizeof(myDIR));
//    if(dir == NULL)
//    {
//        perror("Can't open directory");
//    }
//    dir->dir_index = index;
//    return dir;
//}