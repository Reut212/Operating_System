#include <fcntl.h>
#include "myfs.h"

// checking that you are not able to open a file that is not mounted
void mount_test() {
    mymkfs(10000);
    int fd = myopen("home/reut/file", O_CREAT);
    if (fd == -1) {
        printf("Mount test passed!\n");
    }
}

// checking that you are not able to read , write , seek a file that is not opened
void close_test() {
    mymkfs(10000);
    mymount("filesystem.txt", "output.txt", NULL, 0, NULL);
    int fd = myopen("home/reut/file", O_CREAT);
    myclose(fd);
    int w = mywrite(fd, "blabla", 6);
    char buf[100];
    int r = myread(fd, buf, 50);
    int s = mylseek(fd, 0, SEEK_SET);
    if (w == -1 && r == -1 && s == -1) {
        printf("Close test passed!\n");
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
}


int main() {
    mount_test();
    close_test();
    dir_test();
//    mymkfs(10000);
//    mymount("filesystem.txt", "output.txt", NULL, 0, NULL);
//    int fd = myopen("home/reut/file", O_CREAT);
//    int fd2 = myopen("home/neta/file", O_CREAT);
//    myclose(fd2);
//
//    mywrite(fd, "blabla", 6);
//
//    mywrite(fd, "blabla", 6);
//    mywrite(fd, "bla", 3);
//    int index = open_index(fd);
//    printf("1: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
//    mylseek(fd, 4, SEEK_SET);
//    printf("2: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
//    mylseek(fd, 7, SEEK_SET);
//    printf("3: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
//    mylseek(fd, -2, SEEK_CUR);
//    printf("4: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
//    mylseek(fd, 0, SEEK_CUR);
//    printf("5: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
//    mylseek(fd, 3, SEEK_CUR);
//    printf("6: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
//    mylseek(fd, -2, SEEK_END);
//    printf("7: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
//    mylseek(fd, 2, SEEK_CUR);
//    printf("8: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
//    mylseek(fd, 0, SEEK_END);
//    printf("9: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
//    mylseek(fd, 0, SEEK_SET);
//    printf("10: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
//    char buf[100];
//    myread(fd, buf, 50);
//    printf("%s\n", buf);
//    mylseek(fd, 3, SEEK_SET);
//    printf("11: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
//    memset(buf, 0, 50);
//    myread(fd, buf, 100);
//    printf("%s\n", buf);
//
//    myDIR *dir1 = myopendir("home/reut");
//    myDIR *dir2 = myopendir("home/Amit"); // should print "No such directory"
//    myDIR *dir3 = myopendir("home/reut/file"); // should print "This is a file not a directory!"
//    myDIR *dir4 = myopendir("home");
//    mydirent *dirent1 = myreaddir(dir1);
//    mydirent *dirent2 = myreaddir(dir2);
//    mydirent *dirent3 = myreaddir(dir4);
//    myclose(fd);
}