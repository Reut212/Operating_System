#include <fcntl.h>
#include "myfs.h"

int main(){
    mymkfs(10000);
//    mymount(NULL,"fs_data",NULL,0,NULL);
    int fd = myopen("filesystem.txt", O_CREAT);
    mywrite(fd,"blabla",6);
    mywrite(fd, "bla", 3);
//    mywrite(fd, "bla", 3);
    printf("block: %d, offset:%d\n", open_f[0].current_block_index, open_f[0].current_offset);
    mylseek(fd, 4, SEEK_SET);
    printf("block: %d, offset:%d\n", open_f[0].current_block_index, open_f[0].current_offset);
    mylseek(fd, 7, SEEK_SET);
    printf("block: %d, offset:%d\n", open_f[0].current_block_index, open_f[0].current_offset);
    mylseek(fd, -2, SEEK_CUR);
    printf("block: %d, offset:%d\n", open_f[0].current_block_index, open_f[0].current_offset);
    mylseek(fd, 0, SEEK_CUR);
    printf("block: %d, offset:%d\n", open_f[0].current_block_index, open_f[0].current_offset);
    mylseek(fd, 3, SEEK_CUR);
    printf("block: %d, offset:%d\n", open_f[0].current_block_index, open_f[0].current_offset);
    mylseek(fd, -2, SEEK_END);
    printf("block: %d, offset:%d\n", open_f[0].current_block_index, open_f[0].current_offset);
    mylseek(fd,2,SEEK_CUR);
    printf("block: %d, offset:%d\n", open_f[0].current_block_index, open_f[0].current_offset);
    mylseek(fd, 0, SEEK_END);
    printf("block: %d, offset:%d\n", open_f[0].current_block_index, open_f[0].current_offset);
    myclose(fd);
//    mylseek(y, 0, 0);
//    char buf [100];
//    myread(y, buf, 50);
//    printf("%s\n",buf);
//    myclose(y);
}