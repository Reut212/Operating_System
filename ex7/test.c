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

void fopen_modes_test() {
    mymkfs(10000);
    mymount("filesystem.txt", "output.txt", NULL, 0, NULL);
    myFILE *f1 = myfopen("/home/neta/meow", "ac");
    myFILE *f2 = myfopen("/home/neta/meow", "c");
    myFILE *f3 = myfopen("/home/neta/meow", "c+");
    myFILE *f4 = myfopen("/home/neta/meow", "abc");
    if (f1 == NULL && f2 == NULL && f3 == NULL && f4 == NULL) {
        printf("Fopen modes test passed!\n");
    }
}

void incorrect_modes_test() {
    mymkfs(10000);
    mymount("filesystem.txt", "output.txt", NULL, 0, NULL);
    myFILE *f2 = myfopen("/home/neta/meow", "r"); // can't write
    char buf[100];
    int w2 = myfwrite(buf, 1, 100, f2);
    myFILE *f3 = myfopen("/home/neta/meow", "w"); // can't read
    myFILE *f4 = myfopen("/home/neta/meow", "a"); // can't read
    int r1 = myfread(buf, 1, 100, f3);
    int r2 = myfread(buf, 1, 100, f4);
    if (w2 == -1 && r1 == -1 && r2 == -1) {
        printf("Incorrect modes test passed!\n");
    }
}

void null_test() {
    myFILE *f = myfopen("/home/neta/meow", "ac");
    char buf[100];
    int w = myfwrite(buf, 1, 100, f);
    int r = myfread(buf, 1, 100, f);
    int s = myfseek(f, 0, SEEK_SET);
    int c = myfclose(f);
    int sf = myfscanf(f, "%d");
    int pf = myfprintf(f, "%d");
    if (w == -1 && r == -1 && s == -1 && c == -1 && sf == -1 && pf == -1) {
        printf("Null test passed!\n");
    }
}

void rplus_mode_test() {
    mymkfs(10000);
    mymount("filesystem.txt", "output.txt", NULL, 0, NULL);
    myFILE *f = myfopen("/home/neta/meow", "r+");
    char *str = "this is a test!";
    int w = myfwrite(str, 1, strlen(str), f);
    char output[strlen(str)];
    int s = myfseek(f, 0, SEEK_SET);
    int r = myfread(output, 1, strlen(str), f);
    bool t1 = false, t2 = false, t3 = false, t4 = false;
    if (!strcmp(output, str) && w != -1 && r != -1 && s != -1) {
        t1 = true;
    }
    memset(output, 0, strlen(str));
    int s2 = myfseek(f, -5, SEEK_END);
    int r2 = myfread(output, 1, strlen(str), f);
    if (!strcmp(output, "test!") && r2 != -1 && s2 != -1) {
        t2 = true;
    }
    memset(output, 0, strlen(str));
    int s3 = myfseek(f, 0, SEEK_SET);
    int s4 = myfseek(f, 5, SEEK_CUR);
    int r3 = myfread(output, 1, strlen(str), f);
    if (!strcmp(output, "is a test!") && r3 != -1 && s3 != -1 && s4 != -1) {
        t3 = true;
    }
    memset(output, 0, strlen(str));
    int s5 = myfseek(f, 0, SEEK_SET);
    int r4 = myfread(output, 1, strlen(str), f);
    if (!strcmp(output, "this is a test!") && r4 != -1 && s5 != -1) {
        t4 = true;
    }
    if (t1 == true && t2 == true && t3 == true && t4 == true) {
        printf("r+ test passed!\n");
    }
}


void r_and_w_modes_test() {
    mymkfs(10000);
    mymount("filesystem.txt", "output.txt", NULL, 0, NULL);
    myFILE *write = myfopen("/home/neta/meow", "w");
    myFILE *read = myfopen("/home/neta/meow", "r");
    char *str = "this is a test!";
    char *str2 = " ,THIS IS A TEST!";
    char *str3 = " ,another test..!";

    int w = myfwrite(str, 1, strlen(str), write);
    char output[32];
    int r = myfread(output, 1, strlen(str), read);
    bool t1 = false, t2 = false, t3 = false;
    if (!strcmp(output, str) && w != -1 && r != -1) {
        t1 = true;
    }
    memset(output, 0, 32);
    int w2 = myfwrite(str2, 1, strlen(str2), write);
    int s1 = myfseek(read, 0, SEEK_SET);
    int r2 = myfread(output, 1, 32, read);
    if (!strcmp(output, "this is a test! ,THIS IS A TEST!") && w2 != -1 && r2 != -1 && s1 != -1) {
        t2 = true;
    }
    memset(output, 0, 32);
    int s2 = myfseek(write, 15, SEEK_SET);
    int w3 = myfwrite(str3, 1, strlen(str3), write);
    int s3 = myfseek(read, 0, SEEK_SET);
    int r3 = myfread(output, 1, 32, read);
    if (!strcmp(output, "this is a test! ,another test..!") && s2 != -1 && w3 != -1 && s3 != -1 && r3 != -1) {
        t3 = true;
    }
    if (t1 == true && t2 == true && t3 == true) {
        printf("r and w test passed!\n");
    }
}

int main() {
//    // PART A TESTS
//    mount_test();
//    closed_file_test();
//    unopen_file_test();
//    dir_test();
//    read_write_test();
//    read_write_test_non_continuous_memory_test();
//    seek_non_continuous_memory_test();
//
//    // PART B TESTS
//    fopen_modes_test();
//    incorrect_modes_test();
//    null_test();
//    rplus_mode_test();
    r_and_w_modes_test();
}