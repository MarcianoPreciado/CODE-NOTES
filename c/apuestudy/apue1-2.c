/* 1.6 程序和进程
 */

#include	<stdio.h>
#include	<unistd.h>
#include	<string.h>
#include	<sys/wait.h>
#include	<stdlib.h>

int main(void)
{
        int status;
        char buf[30];
        pid_t pid;

        while (fgets(buf, 29, stdin) != NULL) {
                if (buf[strlen(buf)-1 ] == '\n') {
                        buf[strlen(buf) -1 ] = '\0';
                }

                printf("virtual shell: ");
                if ((pid = fork()) < 0) {
                        perror("fuck\n");
                        exit(126);
                } else if (pid == 0) {
                        execlp(buf, buf, (char *)0);

                        /* 子进程继承父进程的副本，获得父进程的数据空间、堆和栈的副本。exec只是用一个全新的程序替换了当前进程的正文、数据、堆和栈段。所以如果execlp成功，perror这行不会执行，但如果失败，此时仍是在副本之中，执行perror。 */
                        perror("couldn't exec ");
                        exit(127);              /* 退出的是子进程，不会影响整个程序 */
                }

                if ((pid = waitpid(pid, &status, 0)) < 0) {
                        perror("waitpid error\n");
                }
        }
        return 0;
}

/* 在一个进程内的所有线程共享同一地址空间、文件描述符、栈以及相关的属性。需要采取同步措施以避免不一致性。
 */

/* 1.10 时间值
 */
//
// 日历时间：
//	1970/1/1以来国际标准时间（UTC）所经过的秒数累计。 time_t
//
// 进程时间：
//	也被称为CPU时间，以时钟滴答计算。 clock_t
//	用户CPU时间，是执行用户指令所使用的时间。
//	系统CPU时间，是为该进程执行内核程序所经历的时间。如：进程调用read/write，在内核执行read/write所花费的时间。
//	用户CPU时间 + 系统CPU时间 = CPU时间
//
// 实际时间：
//	realtime ?
//
