
#include	<stdio.h>
#include	<signal.h>

volatile sig_atomic_t quitflag;
int sigint_times = 0;

static void sig_int(int signo)
{
        if ( signo == SIGINT) {
                sigint_times++;
                printf("\ninterrupt\n");
                printf("caught sigint times = %d次\n", sigint_times);
        } else if (signo == SIGQUIT) {
                quitflag = 1;
        }
}

int main(void)
{
        sigset_t newmask, oldmask, zeromask;

        if (signal(SIGINT, sig_int) < 0) {
                perror("sigint error");
        }

        if (signal(SIGQUIT, sig_int) < 0) {
                perror("sigint error");
        }

        sigemptyset(&zeromask);
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGQUIT);

        if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
                perror("sigprocmask error");
        }

        while (quitflag == 0) {
                sigsuspend(&zeromask);
        }

        quitflag = 0;
        sigprocmask(SIG_SETMASK, &oldmask, NULL);

        return 0;
}
