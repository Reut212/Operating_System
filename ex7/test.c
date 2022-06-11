#include <fcntl.h>
#include "myfs.h"

int main(){
    mymkfs(10000);
//    mymount(NULL,"fs_data",NULL,0,NULL);
    int fd = myopen("filesystem.txt", O_CREAT);
    mywrite(fd,"blabla",6);
    mywrite(fd, "bla", 3);
//    mywrite(fd, "bla", 3);
//    mylseek(fd, 0, SEEK_CUR);
//    mylseek(fd, 0, SEEK_END);
    mylseek(fd, 4, SEEK_SET); // moving to the left
    mylseek(fd, 7, SEEK_SET);  // moving to the right

    myclose(fd);
//    mylseek(y, 0, 0);
//    char buf [100];
//    myread(y, buf, 50);
//    printf("%s\n",buf);
//    myclose(y);
}