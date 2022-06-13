#include <fcntl.h>
#include "myfs.h"

int main() {
    mymkfs(10000);
//    mymount(NULL,"fs_data",NULL,0,NULL);
    int fd = myopen("home/reut/file", O_CREAT);
//    myclose(fd);
    int fd2 = myopen("home/neta/file", O_CREAT);
    mywrite(fd, "blabla", 6);
    mywrite(fd, "bla", 3);
//    mywrite(fd, "bla", 3);
    int index = open_index(fd);
    printf("1: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
    mylseek(fd, 4, SEEK_SET);
    printf("2: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
    mylseek(fd, 7, SEEK_SET);
    printf("3: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
    mylseek(fd, -2, SEEK_CUR);
    printf("4: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
    mylseek(fd, 0, SEEK_CUR);
    printf("5: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
    mylseek(fd, 3, SEEK_CUR);
    printf("6: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
    mylseek(fd, -2, SEEK_END);
    printf("7: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
    mylseek(fd, 2, SEEK_CUR);
    printf("8: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
    mylseek(fd, 0, SEEK_END);
    printf("9: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
    mylseek(fd, 0, SEEK_SET);
    printf("10: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
    char buf[100];
    myread(fd, buf, 50);
    printf("%s\n", buf);
    mylseek(fd, 3, SEEK_SET);
    printf("11: block: %d, offset:%d\n", open_f[index].current_block_index, open_f[index].current_offset);
    memset(buf, 0, 50);
    myread(fd, buf, 100);
    printf("%s\n", buf);

    myDIR * dir1 =  myopendir("home/reut");
    myDIR * dir2 =  myopendir("home/Amit"); // should print "No such directory"
    myDIR * dir3 =  myopendir("home/reut/file"); // should print "This is a file not a directory!"
    myDIR * dir4 =  myopendir("home");
    mydirent* dirent1 = myreaddir(dir1);
    mydirent* dirent2 = myreaddir(dir2);
    mydirent* dirent3 = myreaddir(dir4);
    myclose(fd);
}