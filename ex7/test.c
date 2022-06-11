#include <fcntl.h>
#include "myfs.h"

int main(){
    mymkfs(10000);
//    mymount(NULL,"fs_data",NULL,0,NULL);
    int y = myopen("filesystem.txt", O_CREAT);
    mywrite(y,"blabla\n",6);
//    mylseek(y, 0, 0);
//    char buf [100];
//    myread(y, buf, 50);
//    printf("%s\n",buf);
//    myclose(y);
}