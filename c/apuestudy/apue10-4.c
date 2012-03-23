
#include 	<stdio.h>
#include	<stdlib.h>
#include	<signal.h>
#include	<unistd.h>

void abort1(void)
{
        sigset_t mask;
        struct sigaction action;

        sigaction(SIGABRT, NULL, &action);

        if (action.sa_handler == SIG_IGN) {     /* POSIX.1要求SIGABRT不可被呼略 */
                printf("mo ren hu lue\n");
                action.sa_handler = SIG_DFL;
                sigaction(SIGABRT, &action, NULL);
        }

        if (action.sa_handler == SIG_DFL) {
                fflush(NULL);
        }

        sigfillset(&mask);
        sigdelset(&mask, SIGABRT);
        sigprocmask(SIG_SETMASK, &mask, NULL);
        printf("1st sigabrt start\n");
        kill(getpid(), SIGABRT);

/* 当进程终止时,系统会关闭所有打开的文件。如果进程捕捉此信号并返回，那么因为进
 * 程可能产生了更多的输出，所以再一次冲洗所有的流。
 */
        printf("2th sigabrt start\n");
        fflush(NULL);
        action.sa_handler = SIG_DFL;
        sigaction(SIGABRT, &action, NULL);
        sigprocmask(SIG_SETMASK, &mask, NULL);
        kill(getpid(), SIGABRT);
        exit(1);
}

int main(void)
{
        abort1();
        return 0;
}
