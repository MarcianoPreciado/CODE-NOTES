/* 第8章：进程控制
 */

/* 父进程设置的文件锁不会被子进程继承
 * 子进程的未处理的闹钟(alarm)被清除
 * 子进程的未处理信号集设置为空集
 */

/*
 * vfork创建子进程的目的是exec一个新程序，并不将父进程的地址空间完全复制到子进程中。
 * 子进程调用exec或exit之前，它在父进程的地址空间中运行。
 * vfork区别于fork：vfork确保子进程先运行，在它调用exec或exit之后父进程才可能被调度运行。
 * 如果在调用这两个函数之前子进程依赖于父进程的进一步动作，则会导致死锁。
 */

/* _exit并不执行标准I/O缓冲的冲洗操作。
 *
 * vfork中，如果子进程调用exit，则该程序的输出是不确定的，它依赖于标准I/O库的实现。
 * 情况1：exit实现冲洗所有标准I/O流，如果这是函数唯一的动作，那么输出与
 * 子进程调用_exit完全相同。
 * 情况2：如果该实现也关闭标准I/O流，那么表示标准输出FILE对象的相关存储区会被清0。
 * 因为子进程借用了父进程的地址空间，所以父进程恢复运行并调用printf时，也就不会产生任何输出。
 *
 *
 *
 *
 * 大多数exit的现代实现不再关闭流，因为进程即将终止，那时内核将关闭在进程中哦
进程中已打开的所有文件描述符，在库中关闭他们。比如Linux

课后题8.1我不明白，为什么fclose(stdout)可行，而close(STDOUT_FILENO)不可行。
*/

/* 对于父进程已经终止的所有进程，他们的父进程都改变为init进程。我们称这些进程
 * 为孤儿进程，由init进程领养。
 * 一个已经终止，但是其父进程尚未对其进行善后处理（获取终止子进程的有关信息，
 * 释放它仍占用的资源）的进程被称为僵死进程。
 */

#include	<sys/wait.h
pid_t wait(int *statloc);
pid_t waitpid(pid_t pid, int *statloc, int options);
/* 如果成功返回则返回进程ID，0。如果出错则返回-1。
 * pid = -1，等待任一子进程。
 * pid > 0，等待其进程ID与pid相等的子进程。
 * pid = 0，等待其组ID等于调用进程组ID的任一子进程。
 * pid = -1,等待其组ID等于pid绝对值的任一子进程。
 *
 * options常量：
 * 1，WCONTINUED 	若实现作业控制，如果PID指定的任意子进程在暂停后已经继
已经继续，但其状态尚未报告，则返回其状态。
 * 2，WNOHANG 		若由PID指定的子进程并不是立即可用的，则waitpid不阻塞，
 * 其返回值为0。
 * 3，WUNTRACED 	若某实现支持作业控制，而由pid指定的任一子进程已处于暂
 *暂停状态，并且自暂停状态以来还未报告过，则返回其状态。
 * * */

WIFEXITED(status)        /* 若为正常终止子进程返回的状态则为真，
			    可执行WEXITSTATUS(status)来获取子进程传给exit族参数的低8位 */
WIFSIGNALED(status)      /* 异常终止状态宏测试。WTERMSIG(status)  WCOREDUMP(status)  */
WIFSTOPPED(status)       /* 暂停子进程返回的状态，WSTOPSIG(status)*/
WIFCONTINUED(status)     /* 作业控制暂停后已经继续的子进程反回状态。 */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

pid_t wait3(int *status, int options, struct rusage *rusage);
pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage);
/* 如果成功则返回进程ID，如果出错则返回-1。
 * rusage为资源统计信息，包括用户CPU时间总量、系统CPU时间总量、页面出错次数，
 * 接受到的信号次数等。有关细节参阅 getrusage(2)手册页。
 */

/* race condition(竞争条件)，polling(轮询)，
 */
#include <unistd.h>

extern char **environ;

int execl(const char *path, const char *arg, ...);
int execlp(const char *file, const char *arg, ...);
int execle(const char *path, const char *arg,
           ..., char * const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execve(const char *path, char *const argv[], char * const envp[]); /* 只有execve是内核系统调用，其它都是库函数 */
/* 若函数执行出错返回-1，否则不返回值。
 *
 * execl、execlp、execle着三个函数表示命令行参数的一般方法是：
 * char *arg0, char *arg1, ..., (char *) 0;
 * 如果用常数0表示一个空指针，必须将它强制转换为一个字符指针，否则将它解释为
 *整形参数。
 *
 * 字母p表示该函数取filename为参数，并且用PATH环境变量寻找可执行文件。
 * 字母l表示该函数取一个参数表。
 * 字母v表示该函数取一个argv[]矢量。
 * 字母e表示该函数取envp[]数组，而不是使用当前环境。
 */

/*
  *
  * 执行新程序的进程还保持了进程的下列特征：
  * 进程ID和父进程ID，实际用户ID和实际组ID，闹钟尚余留时间，当前工作目录，文件
  文件创建屏蔽字，文件锁，进程信号屏蔽，未处理信号，资源限制，tms_utime，tms_stime，tms_cutime，tms_cstime值等。
  * 除非对设置了 打开文件描述符的close-on-exec标志，否则系统默认在执行exec后仍保持这种描述符打开。
  * 有效ID是否改变则取决于所执行程序文件的设置用户ID位和设置组ID位是否设置。
  *
  * All process attributes are preserved during an execve(), except the following:

*      The dispositions of any signals that are being caught are reset to the default (signal(7)).

*      Any alternate signal stack is not preserved (sigaltstack(2)).

*      Memory mappings are not preserved (mmap(2)).

*      Attached System V shared memory segments are detached (shmat(2)).

*      POSIX shared memory regions are unmapped (shm_open(3)).

*      Open POSIX message queue descriptors are closed (mq_overview(7)).

*      Any open POSIX named semaphores are closed (sem_overview(7)).

*      POSIX timers are not preserved (timer_create(2)).

*      Any open directory streams are closed (opendir(3)).

*      Memory locks are not preserved (mlock(2), mlockall(2)).

*      Exit handlers are not preserved (atexit(3), on_exit(3)).

*      The floating-point environment is reset to the default (see fenv(3)).

The  process  attributes  in  the preceding list are all specified in POSIX.1-2001.  The following Linux-specific process attributes are
also not preserved during an execve():

*  The prctl(2) PR_SET_DUMPABLE flag is set, unless a set-user-ID or set-group ID program  is  being  executed,  in  which  case  it  is
   cleared.

*  The prctl(2) PR_SET_KEEPCAPS flag is cleared.

*  The process name, as set by prctl(2) PR_SET_NAME (and displayed by ps -o comm), is reset to the name of the new executable file.

*  The termination signal is reset to SIGCHLD (see clone(2)).
*/

#include	<unistd.h>
int setuid(uid_t uid);
int setgid(uid_t uid);
int seteuid(uid_t uid);
int setegid(uid_t uid);

/* 改变用户ID的规则：
 * 若用户具有超级用户特权，则setuid函数将实际用户ID，有效用户ID以及保存的设置用户ID均设置为uid。
 * 若进程没有超级用户特权，但是uid等于实际用户ID或保存的设置用户id，则setuid将有效用户ID设置为uid，不改变实际用户ID和保存的设置用户ID。
 * 如果以上两个条件都不满足，则将errno设置为EPERM，并返回-1。
 */

/* 关于内核维护的三个用户ID，还要注意以下几点
 * 只有超级用户进程可以改变实际ID。
 * 仅当对程序文件设置了设置用户ID位时，exec函数才会设置有效用户ID。结合以上，
 * 任何时候都可以调用setuid，将有效用户ID设置为实际用户ID或设置用户ID。不能将
 * 有效用户ID设置为任意随机值。*/


int setreuid(uid_t ruid, uid_t euid);
int setregid(gid_t rgid, gid_t egid);
/* 如果其中任一参数的值为-1，则表示相应的ID应当保持不变。
 * 一个非特权用户总能交换实际用户ID和有效用户ID。
 */

/* 当被执行文件是解释器文件时，argv[0]是该解释器的pathname，argv[1]是解释器中
 * 的可选参数，其余参数是exec后的pathname，参数等*/

system()在实现中调用了fork, exec, waitpid，因此有三种返回值。
/* 1，如果fork失败或者waitpid返回除EINTR之外的出错，则返回 -1，而且errno中设置错误类型值。
 * 2，如果exec失败，则其返回值如同shell执行了exit(127)一样。
 * 3，所有三个函数都执行成功，并且system的返回值是shell的终止状态，其格式已在waitpid中说明。
 *
 * shell的 -c选项告诉shell程序取下一个命令行参数作为标准输入，shell将对字符串
 * 进行语法分析，将它们分成命令行参数。
 * 调用_exit是为了防止任一标准IO缓冲区（由父进程复制到子进程）在子进程中被冲洗。
 */

/* apue英文版P248，中文版P202中介绍说在设置用户ID程序中调用system，执行其他程
 * 序，存在安全性漏洞，导致超级用户权限保存下来的问题注释中说，bash v2中不能运
 * 行此实例，经ubuntu 11.04 bash4.28实测，否定，不知原因???*/

/* 如果一个进程正以特殊的权限运行，它又想生成另一个进程执行另一个程序，则它应
 *应当直接fork和exec，且在fork后、exec之前改回普通用户权限。设置用户ID和设置组
 ID程序绝不应当调用system函数*/

/* 进程时间：
 *
 * 任一进程都可调用times函数以获得它自己及终止子进程的墙上时钟时间、用户CPU时
 *时间、系统CPU时间*/

#include	<sys/times.h>

struct tms {
        clock_t tms_utime; /* user CPU time */
        clock_t tms_stime; /* system CPU time */
        clock_t tms_cutime; /* user CPU time, terminated children */
        clock_t tms_cstime; /* system CPU time，terminated children */
};

clock_t times(struct tms *buf);


