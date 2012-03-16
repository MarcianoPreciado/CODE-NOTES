#include "apue.h"
#include <pthread.h>
#include	<error.c>

void
cleanup(void *arg)
{
        printf("cleanup: %s\n", (char *)arg);
}

void *
thr_fn1(void *arg)
{
        printf("thread 1 start\n");
        pthread_cleanup_push(cleanup, "thread 1 first handler");
        pthread_cleanup_push(cleanup, "thread 1 second handler");
        printf("thread 1 push complete\n");
        pthread_cleanup_pop(0);
        pthread_exit((void *)1);
}
int
main(void)
{
        int			err;
        pthread_t	tid1, tid2;
        void		*tret;

        err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
        if (err != 0) {
                err_quit("can't create thread 1: %s\n", strerror(err));
        }

        err = pthread_join(tid1, &tret);
        if (err != 0) {
                err_quit("can't join with thread 1: %s\n", strerror(err));
        }
        printf("thread 1 exit code %d\n", (int)tret);

        exit(0);
}
