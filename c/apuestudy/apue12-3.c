
/*
 * ===  FUNCTION
 * ======================================================================
 * Name:  同步信号处理Description:  普通进程中，唯一可运行的控制线程就是主线程
 *        和信号处理程序，所以阻塞信号
          足以避免错失标志修改。但在线程中，需要使用互斥量来保护标志。

          以下程序，POSIX.1中，异步信号发送到进程以后，进程中没有阻塞该信号的
          被选中，接收。Linux kernel 2.4中，异步信号发送到特定的线程，因为
          Linux每个线程作为独立进程，用clone(2)共享资源，这样系统就不能选择当
          前没有阻塞该信号的线程。这样一来，线程有可能注意不到该信号。信号产生
          于终端驱动程序的话，发送给进程组，下列程序可运行。但如果调用kill的话
          ，Linux就不能如预期的一样工作。

          kernel 2.6实现经我测试和POSIX.1实现一样，没有问题。具体情况还要以后
          探讨。
 * =====================================================================================
 */
#include "apue.h"
#include <pthread.h>
#include	<error.c>

int			quitflag;	/* set nonzero by thread */
sigset_t	mask;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;

void *
thr_fn(void *arg)
{
        int err, signo;

        for (;;) {
                err = sigwait(&mask, &signo);
                if (err != 0) {
                        err_exit(err, "sigwait failed");
                }
                switch (signo) {
                case SIGINT:
                        printf("\ninterrupt\n");
                        break;

                case SIGQUIT:
                        pthread_mutex_lock(&lock);
                        quitflag = 1;
                        pthread_mutex_unlock(&lock);
                        pthread_cond_signal(&waitloc);
                        return(0);

                default:
                        printf("unexpected signal %d\n", signo);
                        exit(1);
                }
        }
}

int
main(void)
{
        int		err;
        sigset_t	oldmask;
        pthread_t	tid;

        sigemptyset(&mask);
        sigaddset(&mask, SIGINT);
        sigaddset(&mask, SIGQUIT);
        if ((err = pthread_sigmask(SIG_BLOCK, &mask, &oldmask)) != 0) { /* 主进程阻塞，线程sigwait会自动解开处理 */
                err_exit(err, "SIG_BLOCK error");
        }

        err = pthread_create(&tid, NULL, thr_fn, 0);
        if (err != 0) {
                err_exit(err, "can't create thread");
        }

        pthread_mutex_lock(&lock);
        while (quitflag == 0) {
                pthread_cond_wait(&waitloc, &lock);
        }
        pthread_mutex_unlock(&lock);

        /* SIGQUIT has been caught and is now blocked; do whatever */
        quitflag = 0;

        /* reset signal mask which unblocks SIGQUIT */
        if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
                err_sys("SIG_SETMASK error");
        }
        exit(0);
}
