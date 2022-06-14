#include <stdio.h>
#include "mystdio.h"
#include "myfs.h"

void action_A() {
    printf("You picked action A!\n");
    printf("Menu: 1- open 2- close 3- write 4- read 5- seek 6- opendir+readdir 7- back to main menu\n");
    while (1) {
        int action = "";
        printf("Pick action\n");
        scanf("%d", &action);
        if (action == 7) {
            break;
        } else if (action == 1) {
            printf("Type the name of file you want to open:\n");
            char file[FILENAME_MAX];
            scanf("%s", file);
            int fd = myopen(file, 0);
            printf("This is your fd: %d\n", fd);
        } else if (action == 2) {
            printf("Type the fd of the file you want to close:\n");
            int fd = -1;
            scanf("%d", &fd);
            int c = myclose(fd);
            if (c != -1) {
                printf("The file has been closed!\n");
            }
        } else if (action == 3) {
            printf("Type the fd of the file you want to write to:\n");
            int fd = -1;
            scanf("%d", &fd);
            printf("Type the string you want to write (until 20 chars):\n");
            char str[20];
            scanf("%s", str);
            int w = mywrite(fd, str, strlen(str));
            if (w != -1) {
                printf("The string has been written!\n");
            }
        } else if (action == 4) {
            printf("Type the fd of the file you want to read from:\n");
            int fd = -1;
            scanf("%d", &fd);
            printf("Type number of bytes you want to read (until 20 chars):\n");
            int num = -1;
            scanf("%d", &num);
            char str[20];
            int r = myread(fd, str, num);
            if (r != -1) {
                printf("The is your string:\n");
                printf("%s\n", str);
            }
        } else if (action == 5) {
            printf("Type the fd of the file you want seek from:\n");
            int fd = -1;
            scanf("%d", &fd);
            printf("1- SEEK_CURR, 2- SEEK_END, 3- SEEK_SET, type your choice:\n");
            int choice = -1;
            scanf("%d", &choice);
            printf("Type how much you want to move:\n");
            int offset = -1;
            scanf("%d", &offset);
            if (choice == 1) {
                mylseek(fd, offset, SEEK_CUR);
            } else if (choice == 2) {
                mylseek(fd, offset, SEEK_SET);
            } else if (choice == 3) {
                mylseek(fd, offset, SEEK_SET);
            } else {
                printf("Invalid action\n");
            }
        } else if (action == 6) {
            printf("Type the name of folder you want to open:\n");
            char folder[FILENAME_MAX];
            scanf("%s", folder);
            myDIR *dir = myopendir(folder);
            myreaddir(dir);
        }
    }
}

void action_B() {
    printf("You picked action B!\n");
    printf("Menu: 1- open 2- close 3- write 4- read 5- seek 6- opendir+readdir 7- back to main menu\n");
}

int main() {
    printf("Hello!\n");
    printf("Menu: A- open/ close/ write/ read/ seek/ opendir/ readdir\n");
    printf("Menu: B- fopen/ fclose/ fwrite/ fread/ fseek/ fscanf/ fprintf\n");
    printf("Menu: C- exit");
    mymkfs(10000);
    mymount("filesystem.txt", "output.txt", NULL, 0, NULL);
    while (1) {
        char action = "";
        printf("Pick A / B / C\n");
        scanf("%c", &action);
        if (action == 'C') {
            printf("Bye!");
            break;
        }
        if (action == 'A') {
            action_A();
        }
        if (action == 'B') {
            action_B();
        }
    }
}