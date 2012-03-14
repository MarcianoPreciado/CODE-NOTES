
#include 	<stdio.h>
#include	<stdlib.h>
#include	<signal.h>
#include	<unistd.h>

void abort1(void)
{
        sigset_t mask;
        struct sigaction action;

        sigaction(SIGABRT, NULL, &action);

        if (action.sa_handler == SIG_IGN) {
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

        printf("2th sigabrt start\n");
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
