#include	<signal.h>

int sigaction(int signo, const struct sigaction *restrict act, struct sigaction *restrict buf);

struct sigaction {
        void     (*sa_handler)(int);
        void     (*sa_sigaction)(int, siginfo_t *, void *);
        sigset_t   sa_mask;
        int        sa_flags;
        void     (*sa_restorer)(void);
};
/* sa_mask字段说明了一个信号集，在调用信号捕捉函数之前，这一信号集要加到进程的
进程的信号屏蔽字中。仅当从信号捕捉函数返回时再将进程的信号屏蔽字复原。
 *
           SA_NOCLDSTOP
                   进程停止时（例如接收到SIGSTOP，SIGSTP，SIGTTIN 或 SIGTTOU信号停止）
                   或收到继续（SIGCONT）时不产生SIGCHLD信号，进程终止时仍然产
                   生SIGCHLD信号。

           SA_NOCLDWAIT (since Linux 2.6)
                  如果signum是SIGCHLD，则在子进程终止时不创建僵尸进程

                  If the SA_NOCLDWAIT flag is set when establishing a handler for
                  SIGCHLD, POSIX.1 leaves it unspecified whether a SIGCHLD signal
                  is  generated  when  a  child  process terminates.  On Linux, a
                  SIGCHLD signal is generated in this case; on some other  imple-
                  mentations, it is not.

           SA_NODEFER
                  当捕捉到signum信号，并执行信号处理程序时，系统不自动阻塞此信号。
                  注意：针对早期不可靠信号。

           SA_ONSTACK
                  Call the signal handler on an alternate signal  stack  provided
                  by sigaltstack(2).  If an alternate stack is not available, the
                  default stack will be used.  This flag is only meaningful  when
                  establishing a signal handler.

           SA_RESETHAND
                  Restore  the signal action to the default state once the signal
                  handler has been called.  This flag  is  only  meaningful  when
                  establishing a signal handler.  SA_ONESHOT is an obsolete, non-
                  standard synonym for this flag.

           SA_RESTART
                  由此信号处理中断的系统调用会自动重启动。

           SA_SIGINFO (since Linux 2.2)
                  The  signal  handler takes 3 arguments, not one.  In this case,
                  sa_sigaction should be set instead of sa_handler.  This flag is
                  only meaningful when establishing a signal handler.

系统支持实时扩展时，由SA_SIGINFO设置的sa_sigaction信号处理程序将导致信号可靠的排队。
 */

/* Freebsd和Mac OS中，setjmp和longjmp函数保存和恢复信号屏蔽字，但其中的_setjmp
 * 和_longjmp与Linux和Solaris 9中的setjmp，longjmp不执行此操作。
 */

/* 在信号处理程序中应当使用sigsetjmp与siglongjmp */


#include	<setjmp.h>

int sigsetjmp(sigjmp_buf env, int savemask);
/* 直接调用返回0，从siglongjmp返回则返回非0值，
 * 如果savemask非0，则sigsetjmp在env中保存进程的当前信号屏蔽字。
 * 调用siglongjmp时，如果sigsetjmp已经保存了env,则siglongjmp从其中恢复保存的信
保存的信号屏蔽字。
*/
int siglongjmp(sigjmp_buf env, int val);

/* 程序清单10-14
 * 在信号处理程序中调用siglongjmp，应当使用如下技术：
 * 仅在调用sigsetjmp后才将变量canjump设置为非0值。
 * 在信号处理程序中检测次变量，仅当它为非0值时才调用siglongjmp。
 * 这提供了一种保护机制，使得在jmbuf（跳转缓冲）尚未由sigsetjmp初始化时，防止
 * 调用信号处理程序。
 *
 * 数据类型 sig_atomic_t，由ISO C标准定义，在写这种类型的变量时不会被中断。
 * 这意味着在具有虚拟存储器的系统上这种变量不会跨越页边界，可用一条机器指令。
 * 这种类型的变量总是包括ISO类型修饰符volatile，其原因是：该变量将由两个不同的
控制线程——main函数和异步执行的信号处理程序访问。
*/

int sigsuspend(const sigset_t *sigmask);         /* 始终返回-1，errno设置为EINTR */

/* 当调用一个信号处理程序时，被捕捉到的信号加到进程的当前信号屏蔽字中。当从信
 * 号处理程序返回时，恢复原来的屏蔽字 */

int siglongjmp(sigjmp_buf env, int val);


#include	<stdlib.h>

void abort(void);
/* POSIX.1要求如果abort调用终止进程，则它对所有打开的标准I/O流的效果应当与进程
 * 终止前对每个流调用fclose相同。 POSIX.1也说明abort并不理会进程对此信号的阻塞
 * 和忽略。
 * 让进程捕捉SIGABRT信号的意图是：在进程中止之前由进程执行所需的清理操作。
 * */

/* POSIX.1说明：若父进程捕捉SIGCHLD信号，那么正在执行system函数时，应当阻塞对
 * 父进程递送SIGCHLD信号。否则，当system函数创建的子进程结束时，system的调用者
 * 可能错误认为，它自己的一个子进程已经结束了。于是，调用者调用WAIT函数以获得
 * 子进程终止状态时，组织了system()函数获得子进程的终止状态。
 */

/* system函数的实现需要注意：sigation忽略掉父进程的SIGINT，SIGQUIT信号，
 * sigprocmask block掉SIGCHLD，然后子进程在exec前恢复SIGINT\SIGQUIT默认处理程序，
 * 恢复原来的mask值。然后父进程调用wait系列函数获取状态，恢复SIGINT，SIGQUIT，
 * mask值，并返回子进程退出状态。
 *
 * system返回值是shell终止状态，但shell的终止状态并不总是执行命令字符串进程的
 * 终止状态。bash中有意个特性，如果用退出键或其他信号杀死执行命令进程后，其中
 *终止状态是128+信号编号。该信号终止了正在执行的命令。
 * 例如 sh -c "sleep 30"，后，用ctrl+c中断掉,返回状态为130。
 *
 * 当shell本身异常终止时，system的返回值才报告一个异常终止。
 */

/* POSIX.1认为有6个信号与作业控制有关。
 *
 * SIGCHLD 	子进程已停止或终止。
 * SIGCONT 	如果进程已停止，则使其继续运行。
 * SIGSTOP 	停止信号（不能被捕捉或忽略）。
 * SIGTSTP 	交互式停止信号。
 * SIGTTIN 	后台进程组成员读控制终端。当输入Ctrl+z时（挂起），SIGTSTP被送至
 * 前台进程组的所有进程。
 * SIGTTOU 	后台进程组成员写到控制终端
 *
 * 除SIGCHLD以外，大多数应用程序并不处理这些信号，交互式SHELL通常做所有工作。
 * 作业控制信号有某种交互作用。当对一个进程产生四种停止信号（SIGTSTP, SIGSTOP,
 * SIGTTIN或SIGTTOU）中的任意一种时，对同一进程的任一未决SIGCONT信号将被丢弃。
 * 与此类似，对一个进程产生SIGCONT信号时，对同一进程的任一未决停止信号将被丢弃
 */

/*  某些系统提供数组extern char *sys_siglist[]；
 *  数组下表是信号编号，给出一个指向信号字符串名字的指针。 */

#include	<signal.h>
void psignal(int signo, const char *msg);       /* 类似于perror */
#include	<string.h>
char *strsignal(int signo);                     /* 返回该信号说明的字符串 */

