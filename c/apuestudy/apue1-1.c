/*  第一章： UNIX基础知识

内核的接口被称为系统调用（system call）
shell是一种特殊的应用程序，它为运行其他应用程序提供了一个接口

*/

#include <stdio.h>
#include	<dirent.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
        DIR *dp;
        struct dirent *dirp;

        if (argc != 2) {
                printf("fuck damn\n");
                exit(1);
        }

        if ((dp = opendir(argv[1])) == NULL) {
                perror("fuck you");
        }

        while ((dirp = readdir(dp)) != NULL) {
                printf("%s\n", dirp->d_name);
        }

        closedir(dp);
        return 0;
}

// for和跟随其后的exec两者的组合是某些操作系统所称的产生（spawn）一个新进程。
