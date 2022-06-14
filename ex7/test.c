#include <fcntl.h>
#include "myfs.h"
#include "mystdio.h"

// checking that you are not able to open a file that is not mounted
void mount_test() {
    mymkfs(10000);
    int fd = myopen("home/reut/file", O_CREAT);
    if (fd == -1) {
        printf("Mount test passed!\n");
    }
}

// checking that you are not able to read , write , seek on a closed file
void closed_file_test() {
    mymkfs(10000);
    mymount("filesystem.txt", "output.txt", NULL, 0, NULL);
    int fd = myopen("home/reut/file", O_CREAT);
    myclose(fd);
    int w = mywrite(fd, "blabla", 6);
    char buf[100];
    int r = myread(fd, buf, 50);
    int s = mylseek(fd, 0, SEEK_SET);
    if (w == -1 && r == -1 && s == -1) {
        printf("Closed file test passed!\n");
    }
}

// checking that you are not able to read , write , seek , close on a file that has never been opened
void unopen_file_test() {
    mymkfs(10000);
    mymount("filesystem.txt", "output.txt", NULL, 0, NULL);
    int fd = 2;
    int w = mywrite(fd, "blabla", 6);
    char buf[100];
    int r = myread(fd, buf, 50);
    int s = mylseek(fd, 0, SEEK_SET);
    int c = myclose(fd);
    if (w == -1 && r == -1 && s == -1 && c == -1) {
        printf("Unopen file test passed!\n");
    }
}

// checking that you are not able to opendir a file (that is not a directory)
// and that you can't open a directory that doesn't exist
void dir_test() {
    mymkfs(10000);
    mymount("filesystem.txt", "output.txt", NULL, 0, NULL);
    int fd = myopen("home/reut/file", O_CREAT);
    myDIR *dir = myopendir("home/reut/file");
    myDIR *dir2 = myopendir("home/neta");
    mydirent *empty = myreaddir(dir);
    if (dir == NULL && dir2 == NULL && empty == NULL) {
        printf("Dir test passed!\n");
    }
    myclose(fd);
}

void read_write_test() {
    mymkfs(10000);
    mymount("filesystem.txt", "output.txt", NULL, 0, NULL);
    int fd = myopen("home/reut/file", O_CREAT);
    bool t1, t2, t3 = false;
    char buf[100];
    mywrite(fd, "abc", 3);
    myread(fd, buf, 50);
    if (strcmp(buf, "abc")) {
        t1 = true;
    }
    memset(buf, 0, 100);
    mywrite(fd, "defgh", 5);
    if (strcmp(buf, "abcdefgh")) {
        t2 = true;
    }
    mywrite(fd, "i", 6);
    memset(buf, 0, 100);
    if (strcmp(buf, "abcdefghi")) {
        t3 = true;
    }
    if (t1 == true && t2 == true && t3 == true) {
        printf("Simple read write test passed!\n");
    }
}

void read_write_test_non_continuous_memory_test() {
    mymkfs(10000);
    mymount("filesystem.txt", "output.txt", NULL, 0, NULL);
    int fd = myopen("home/reut/file", O_CREAT);
    int fd2 = myopen("home/neta/file", O_CREAT);
    bool t1, t2, t3 = false;
    char buf[100];
    mywrite(fd, "abc", 3);
    mywrite(fd2, "abc", 3);
    myread(fd, buf, 50);
    if (strcmp(buf, "abc")) {
        t1 = true;
    }
    memset(buf, 0, 100);
    mywrite(fd, "defgh", 5);
    mywrite(fd2, "abc", 3);
    if (strcmp(buf, "abcdefgh")) {
        t2 = true;
    }
    mywrite(fd, "i", 6);
    mywrite(fd2, "abc", 3);
    memset(buf, 0, 100);
    if (strcmp(buf, "abcdefghi")) {
        t3 = true;
    }
    if (t1 == true && t2 == true && t3 == true) {
        printf("Hard read write test passed!\n");
    }
}

void seek_non_continuous_memory_test() {
    mymkfs(10000);
    mymount("filesystem.txt", "output.txt", NULL, 0, NULL);
    int fd = myopen("home/reut/file", O_CREAT);
    // opening another file make the memory be non continuous
    int fd2 = myopen("home/neta/file", O_CREAT);
    mywrite(fd, "blabla", 6);
    mywrite(fd2, "Unrelated string", 16);
    mywrite(fd, "bla", 3);
    myclose(fd2);
    int index = open_index(fd);
    bool t1 = false, t2 = false, t3 = false, t4 = false, t5 = false, t6 = false,
            t7 = false, t8 = false, t9 = false, t10 = false, t11 = false, t12 = false;
    if (open_f[index].current_block_index == 5 && open_f[index].current_offset == 4) {
        t1 = true;
    }
    mylseek(fd, 4, SEEK_SET);
    if (open_f[index].current_block_index == 2 && open_f[index].current_offset == 4) {
        t2 = true;
    }
    mylseek(fd, 7, SEEK_SET);
    if (open_f[index].current_block_index == 5 && open_f[index].current_offset == 2) {
        t3 = true;
    }
    mylseek(fd, -3, SEEK_CUR);
    if (open_f[index].current_block_index == 2 && open_f[index].current_offset == 4) {
        t4 = true;
    }
    mylseek(fd, 0, SEEK_CUR);
    if (open_f[index].current_block_index == 2 && open_f[index].current_offset == 4) {
        t5 = true;
    }
    mylseek(fd, 3, SEEK_CUR);
    if (open_f[index].current_block_index == 5 && open_f[index].current_offset == 2) {
        t6 = true;
    }
    mylseek(fd, -2, SEEK_END);
    if (open_f[index].current_block_index == 5 && open_f[index].current_offset == 2) {
        t7 = true;
    }
    mylseek(fd, 2, SEEK_CUR);
    if (open_f[index].current_block_index == 5 && open_f[index].current_offset == 4) {
        t8 = true;
    }
    mylseek(fd, 0, SEEK_END);
    if (open_f[index].current_block_index == 5 && open_f[index].current_offset == 4) {
        t9 = true;
    }
    // needs to allocate new blocks and not throw error! also not change the size of the file
    // according to the c man
    mylseek(fd, 2, SEEK_END);
    if (open_f[index].current_block_index == 9 && open_f[index].current_offset == 1) {
        t10 = true;
    }
    mylseek(fd, 9, SEEK_SET);
    if (open_f[index].current_block_index == 5 && open_f[index].current_offset == 4) {
        t11 = true;
    }
    // the end is still 9 and no new block should be allocated
    mylseek(fd, 2, SEEK_END);
    if (open_f[index].current_block_index == 9 && open_f[index].current_offset == 1) {
        t12 = true;
    }
    if (t1 == true && t2 == true && t3 == true && t4 == true && t5 == true && t6 == true
        && t7 == true && t8 == true && t9 == true && t10 == true && t11 == true && t12 == true) {
        printf("Seek test passed!\n");
    }
}

void fopen_test() {
    myFILE *f1 = myfopen("/home/neta/meow", "ac");
    myFILE *f2 = myfopen("/home/neta/meow", "c");
    myFILE *f3 = myfopen("/home/neta/meow", "c+");
    myFILE *f4 = myfopen("/home/neta/meow", "abc");
    if (f1 == NULL && f2 == NULL && f3 == NULL && f4 == NULL) {
        printf("Fopen test passed!\n");
    }
}

void incorrect_modes_test() {
    myFILE *f1 = myfopen("/home/neta/meow", "r+");
    myFILE *f2 = myfopen("/home/neta/meow", "r");
    char buf[100];
    int w1 = myfwrite(buf, 1, 100, f1);
    int w2 = myfwrite(buf, 1, 100, f2);
    myFILE *f3 = myfopen("/home/neta/meow", "w");
    myFILE *f4 = myfopen("/home/neta/meow", "a");
    int r1 = myfread(buf, 1, 100, f3);
    int r2 = myfread(buf, 1, 100, f4);
    if (w1 == -1 && w2 == -1 && r1 == -1 && r2 == -1) {
        printf("Incorrect modes test passed!\n");
    }
}

void null_test(){
    myFILE *f = myfopen("/home/neta/meow", "ac");
    char buf[100];
    int w = myfwrite(buf, 1, 100, f);
    int r = myfread(buf, 1, 100, f);
    int s = myfseek(f, 0, SEEK_SET);
    int c = myfclose(f);
    int sf = myfscanf(f, "%d");
    int pf = myfprintf(f, "%d");
    if (w == -1 && r == -1 && s == -1 && c == -1 && sf ==-1 && pf == -1){
        printf("Null test passed!\n");
    }
}

int main() {
    // PART A TESTS
    mount_test();
    closed_file_test();
    unopen_file_test();
    dir_test();
    read_write_test();
    read_write_test_non_continuous_memory_test();
    seek_non_continuous_memory_test();

    // PART B TESTS
    fopen_test();
    incorrect_modes_test();
    null_test();
}