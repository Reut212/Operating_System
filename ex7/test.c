#include <fcntl.h>
#include "myfs.h"

int main() {
    mymkfs(10000);
//    mymount(NULL,"fs_data",NULL,0,NULL);
    int fd = myopen("home/reut/file", O_CREAT);
    myclose(fd);
    int fd2 = myopen("home/neta/file", O_CREAT);
    mywrite(fd2, "blabla", 6);
    mywrite(fd2, "bla", 3);
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
    /* Desired print:
    1: block: 6, offset:1
    2: block: 4, offset:0
    3: block: 5, offset:1
    4: block: 4, offset:1
    5: block: 4, offset:1
    6: block: 6, offset:0
    7: block: 5, offset:1
    8: block: 6, offset:1
    9: block: 6, offset:1
    10: block: 2, offset:0
    */
    // TODO: change when increase block size at the end
    char buf[100];
    myread(fd, buf, 50);
    printf("%s\n", buf);
    mylseek(fd, 3, SEEK_SET);
    memset(buf, 0, 0);
    myread(fd, buf, 50);
    printf("%s\n", buf);
    myclose(fd);
}