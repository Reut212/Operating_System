#include "myfs.h"


void mymkfs(int fs_size) {
    sb.is_mounted = false;
    int size = fs_size - (int) sizeof(sb);
    sb.num_inodes = (int) ((size * 0.1) / sizeof(inode));
    sb.num_blocks = (int) ((size * 0.9) / sizeof(disk_block));
    sb.size_blocks = (int) ((size * 0.9));
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    for (int i = 0; i < sb.num_inodes; i++) {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        memset(inodes[i].name, 0, NAME_SIZE+1);
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
    sb.is_mounted = true;
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
        open_f[open_index].fd = inode_index;
        open_f[open_index].current_offset = 0;
        open_f[open_index].current_block_index = inodes[inode_index].first_block;
        return inode_index;
    }
        // doesn't exist - create and insert to open_f
    else {
        return create_file(filename, flags, isfile, path);
    }
}

// pathname example: /home/reut/CS.txt
int myopen(const char *pathname, int flags) {
    if (!sb.is_mounted) {
        perror("You can't open a file you didn't mount!");
        return -1;
    }
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
        memset(newBuffer, 0, 0);
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
    if (index == -1){
        perror("You are trying to close from a file that is not open!");
        return -1;
    }
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

void myseek(int index, int offset, int how_much_bytes_to_move) {
    int curr = inodes[open_f[index].file_inode].first_block;
    for (int i = 0; i < how_much_bytes_to_move / BLOCKSIZE; i++) {
        if (d_block[curr].next_block_num == -2) {
            alloc_new_block(curr);
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
    while (1) {
        if (curr_block == stop_block && curr_offset == stop_offset) {
            break;
        }
        curr_offset++;
        if (curr_offset == BLOCKSIZE) {
            curr_offset = 0;
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
    while (1) {
        if (d_block[curr_block].data[curr_offset] == 0) {
            break;
        }
        curr_offset++;
        if (curr_offset == BLOCKSIZE) {
            curr_offset = 0;
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
        int how_much_bytes_to_move = (int) offset;
        myseek(index, offset, how_much_bytes_to_move);
        return how_much_bytes_to_move;
    } else if (whence == SEEK_CUR) {
        int where_we_are = where_we_are_now(index);
        myseek(index, offset, where_we_are + (int) offset);
        return where_we_are + (int) offset;
    } else if (whence == SEEK_END) {
        int end = calc_end(index);
        myseek(index, offset, end + (int) offset);
        return end + (int) offset;
    }
    return -1;
}

ssize_t myread(int myfd, void *buf, size_t count) {
    int index = open_index(myfd);
    if (index == -1) {
        perror("You are trying to read from a file that is not open!");
        return -1;
    }
    if (buf == NULL){
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
        if (d_block[curr_block].data[offset] == 0) { // end of file content
            break;
        }
        ((char *) buf)[i] = d_block[curr_block].data[offset];
        offset++;
        bytes_read++;
    }
    mylseek(open_f[index].fd, bytes_read, SEEK_CUR);
    return bytes_read;
}

ssize_t mywrite(int myfd, const void *buf, size_t count) {
    int index = open_index(myfd);
    if (index == -1) {
        perror("You are trying to write to a file that is not open!");
        return -1;
    }
    if (buf == NULL){
        return -1;
    }
    int curr_block = open_f[index].current_block_index; // find the block number
    int offset = open_f[index].current_offset;
    char *data = (char *) buf;
    int new_blocks_allocated = 0;
    int bytes_written =0;
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
        bytes_written++;
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
    return bytes_written;
}

int find_dir_inode(const char *name, const char *path) {
    for (int i = 0; i < sb.num_inodes; i++) {
        if (inodes[i].first_block!=-1) {
            if (!strcmp(inodes[open_f[i].file_inode].name, name)) {
                if (!strcmp(inodes[open_f[i].file_inode].path, path)) {
                    if (inodes[i].file) {
                        perror("This is a file not a directory!");
                        return -1;
                    }
                }
                return i;
            }
        }
    }

    return -1;
}

myDIR *myopendir(const char *name) {
    char str[BUFF_SIZE];
    strcpy(str, name);
    int len = strlen(name);
    char *curr_tok = strtok(str, "/");
    char *last_tok = curr_tok;
    char *arr[len];
    int i = 0;
    int newSize = 0;
    while (curr_tok) {
        last_tok = curr_tok;
        newSize += strlen(curr_tok) + 1;
        arr[i] = curr_tok;
        curr_tok = strtok(NULL, "/");
        i++;
    }
    newSize -= (strlen(last_tok) + 1);
    char *path = (char *) malloc(newSize);
    memset(path, 0, 0);
    for (int j = 0; j < i - 1; j++) {
        strcat(path, "/");
        strcat(path, arr[j]);
    }

    int index = find_dir_inode(last_tok, path);
    if (index == -1) {
        perror("No such directory");
        return NULL;
    }
    myDIR *dir = (myDIR *) malloc(sizeof(myDIR));
    if (dir == NULL) {
        perror("Not enough space");
    }
    dir->dir_inode_index = index;
    free(path);
    return dir;
}

void find_subfiles(mydirent *dirent, char *path) {
    for (int i = 0; i < sb.num_inodes; i++) {
        if (strstr(inodes[i].path, path)) {
            dirent->files_inode_indexes[dirent->size] = i;
            dirent->size++;
            if (dirent->size == MAX_DIR) {
                break;
            }
        }
    }
}

struct mydirent *myreaddir(myDIR *dirp) {
    if (dirp == NULL) {
        perror("Null pointer exepction");
        return NULL;
    }
    mydirent *dirent = (mydirent *) malloc(sizeof(mydirent));
    for (int i = 0; i < MAX_DIR; i++) {
        dirent->files_inode_indexes[i] = -1;
    }
    strcpy(dirent->d_name, inodes[dirp->dir_inode_index].name);
    int newSize = strlen(inodes[dirp->dir_inode_index].path) + strlen(dirent->d_name) + 1;
    char *path = (char *) malloc(newSize);
    memset(path, 0, 0);
    strcpy(path, inodes[dirp->dir_inode_index].path);
    strcat(path, "/");
    strcat(path, dirent->d_name);
    find_subfiles(dirent, path);
    free(path);
    return dirent;
}