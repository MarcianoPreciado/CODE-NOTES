/* 第10章：信号
 */

/* 信号是一种软件中断，它提供了一种处理异步事件的方法。
 * kill函数对信号编号0有特殊的应用，POSIX.1将此种信号编号值称为空信号。
 * 如果signo参数是0，则kill仍执行正常的错误检查，但不发送信号，常被用来确定一
确定一个特定进程是否仍旧存在。如果并不存在，则kill返回-1，并将errno设为 ESRCH。

 * kill(2)函数可将信号发送给另一个进程或进程组。但！接收信号进程和发送信号进程
 * 的所有者必须相同，或者发送信号进程的所有者为超级用户。SIGKILL和SIGSTOP信号
 * 不能被忽略或者被捕捉。
 * signal函数由ISO C定义，因为ISO C不涉及多线程、进程组以及终端I/O，所以它对信
 * 号的定义不明确，最好使用sigaction函数。
 */

/* SIGHUP信号：
 * 如果终端接口检测到一个连接断开，则将此信号发送给与终端相关的控制进程（会话
 * 首进程）。
 * 接到此信号的会话首进程可能在后台，有别于中断、挂起和退出。后者信号总传递给
 * 前台进程组。
 * 如果会话首进程终止，则也产生此信号。这种情况下，此信号被发送给前台进程组中
 * 的每一个进程。*/

/* SIGSTSTP信号：
 * 交互式停止信号。CTRL+Z */

#include	<signal.h>

void (*signal(int signum, void (*func)(int)))(int);
/* 若成功返回信号以前的处理配置（之前的信号处理程序的指针），若出错则返回SIG_ERR。
 *
 * 子进程继承父进程的信号处理方式，exec函数将原先设置要捕捉的函数都更改为它们的默认动作。
 */

if (signal(SIGINT, SIG_IGN) != SIG_IGN)
{
        signal(SIGINT, sig_int);        /* 仅当信号当前未被忽略时，进程才会捕捉它们。 */
}

/* 早期UNIX的一个特性是：如果进程在执行一个低速系统调用而阻塞期间捕捉到一个信
 * 信号，则该系统调用就被中断不再继续执行。该系统调用返回出错，其errno被设置为EINTR。
 *
 * 早期UNIX版本中的一个问题是：在进程每次接收到信号并对其进行处理时，随即将信
 *信号动作复位为默认值。
 *
 * 必须区分系统调用和函数！当捕捉到某个信号时，被中断的是内核中的系统调用。
 *
 * 低速系统调用包括：
 * 1，在读某些类型的文件时（管道，终端设备，网络设备）时，如果数据并不存在则可
 * 可能会使调用者永远阻塞。
 * 2，在写这些类型的文件时，如果不能立即接受这些数据，也可能会使调用者永远阻塞。
 * 3，打开某些类型的文件，在某中条件发生之前（如打开终端设备，它要等待直到调制
 调制解调器应答了电话）。
 * 4，pause和wait函数
 * 5，某些ioctl操作和某些进程间通信函数。
 *
 * FreeBSD,Linux,Mac OS的默认方式是重启动由信号中断的系统调用，Solaris 9默认EINTR。
 *
 */
/*
 * 不可重入函数原因：
 * 1，已知它们使用静态数据结构。
 * 2，它们调用malloc 或 free
 * 3，他们是标准I/O函数。
 * 各种wait函数都能改变errno值。
 */

/* 如果特地设置信号SIGCLD SIGCHLD的捕捉配置为SIG_IGN，则调用进程的子进程不产生
 * 僵死进程。(FreeBSD除外）
 *
 * 在产生了信号时，内核通常在进程表中设置一个某种形式的标志——递送。
 * 在信号产生 (generation) 和递送 (delivery)之间的时间间隔内，我们称信号是未决的(pending)
 *
 * 内核在递送一个原来被阻塞的信号给进程时（而不是在产生该信号时），才决定对他
 对它的处理方式。
 * POSIX.1的Rationale建议：在其他信号之前递送与进程当前状态有关的信号，如SIGSEGV。
 *
 */

#include	<signal.h>
int kill(pid_t pid, int signo);
int raise(int signo);                           /* 成功返回0,否则返回-1 */
/*
 * kill的pid参数：
 * pid > 0 	将该信号发送给进程ID为pid的进程
 * pid = 0 	将信号发送给与发送进程属于同一进程组的所有进程。但不包括实现
 * 定义的系统进程集。（包括内核进程和 init(pid 1) ）
 * pid == -1 	将该信号发送给发送进程有权限向他们发送信号的系统上的所有进程。
 * pid < 0 	发送给其进程组ID等于pid的绝对值。
 *
 * SIGCONT例外，进程可将它发送给属于同一会话的任何进程。
 * */

#include	<unistd.h>
unsigned int alarm(unsigned int seconds);
/* 返回值为0或以前设置的闹钟时间的余留秒数。
 * 每个进程只能有一个闹钟时间。
 */

int pause(void);
/* 使调用进程挂起直至捕捉到一个信号。
 * 只有执行了一个信号处理程序并返回时才返回，值为-1，并将errno设置为EINTR。*/

/* 信号处理程序中，如果该信号中断了其他信号处理程序，且使用longjump避免信号与
 * pause等的竞争条件，则有可能会提早终止其他信号处理程序。
 *
 * 如果要对I/O操作设置时间限制，则可使用longjmp或者使用select和poll函数。
 */

sigset_t set;
*set |= 1 << (signo - 1);  /* sigaddset的一种实现 */
*set &= ~(1 << (signo - 1)); /* sigdelset的一种实现 */

int sigprocmask(int how, sigset_t *restrict set, sigset_t *restrict oset);
/* SIG_BLOCK ，当前信号屏蔽字与set指向信号集的屏蔽字的并集。
 * SIG_UNBLOCK，当前信号屏蔽字与 set指向信号补集 的并集。
 * 而SIG_SETMASK，进程的屏蔽字将被set所指向的信号集的值代替。
 * sigprocmask仅为单线程的进程定义。
 */

#include <signal.h>>
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signo);
int sigdelset(sigset_t *set, int signo);        /* 以上函数，成功0,否则-1 */
int sigismember(const sigset_t *set, int signo); /* 若真1,假0,出错-1 */

int sigpending(sigset_t *set); /* 返回当前阻塞而不能递送的信号集，即未决信号集 */
