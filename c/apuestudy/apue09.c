/* 第9章：进程关系
 */

/* 每个进程都有一个父进程（初始的内核进程并无父进程，也可以说其父进程就是自己
 * BSD终端登录，init进程读取/etc/ttys
 * Linux终端登录，init进程读取/etc/inittab，但upstart(ubuntu) init进程读取
 * /etc/init/tty*文件
 *
 * 传统的UNIX系统用户身份验证过程如下：
 * 系统自举时，内核创建进程为1的init进程，读取终端配置文件，对每一个允许登录的
 * 终端设备，init以空环境调用一次fork，它所生成的子进程执行(exec)getty程序。
 *
 * getty为终端设备调用open函数，以读写方式打开终端。输出login:   之类的信息。
 * 然后它以类似于下面的方式调用login程序。
 * execle("/bin/login", "login", "-p", username, (char *)0, envp);
 * getty以终端名和在gettytab中说明的字符串为login创建一个环境(envp)。
 * -p通知login保留传送给它的环境。
 *
 * login调用getpwnam取得相应用户的口令文件登录项，然后getpass，getpass再调用
 * crypt将用户键入的口令加密与该用户在阴影文件口令中登录项的pw_passwd字段相比相比较。
 *
 * 现代UNIX都支持PAM（可插入式身份验证模块）。
 * */

/* 如果用户登录正确，login将执行如下工作：
 * 当前工作目录更改为用户的起始目录（chdir）
 * 调用chown改变该终端的所有权，使登录用户称为它的所有者
 * 将对该终端设备的访问权改变成用户读和写
 * 调用setgid及initgroups设置进程的组ID
 * 用login所得到的所有信息初始化环境
 * login进程改变为登录用户的用户ID(setuid)并调用该用户的登录shell，如下：*/

execl("bin/sh", "-sh", (char *)0);
/* argv[0]的第一个字符'-'是一个标志，表示该shell被调用为登录shell。
 */

#include	<unistd.h>
pid_t getpgrp(void);

pid_t getpgid(pid_t pid); /* 若pid为0，则返回调用进程的进程组ID。 */
int setpgid(pid_t pid, pid_t pgid);
/* setpgid函数将pid进程的进程组ID改为pgid。如果两个参数相等，则pid变成进程组组长。
 * 如果pid是0，则使用调用者的进程ID。如果pgid是0，则由pid指定的进程ID将用作进程组ID。
 * 一个进程只能为它自己或它的子进程设置进程组ID。*/

pid_t setsid(void);
/* 如果调用此函数的进程不是一个进程组的组长，则此函数创建一个进程
 * 结果将发生以下三件事：
 * 1,该进程变成会话首进程
 * 2,该进程成为一个进程组的组长进程。
 * 3,该进程没有控制终端。如果在setsid之前该进程有一个控制终端，那么这种联系会切
那么这种联系会切断。 */

pid_t getsid(pid_t pid);
/* 如果pid是0，getsid返回调用进程的会话首进程的进程组ID。
 * Linux之外有些系统如果pid不属于调用者所在的会话，则产生EPRM错误。*/

/* 建立与控制终端连接的会话首进程被称为控制进程（controlling process）。
 * 一个会话中的几个进程组可分成一个前台进程组 (foreground process group)，以及
 * 一个或几个后台进程组 (background process group)。
 * 无论何时键入终端的中断键 (Ctrl+C) 或终端的退出键 (Ctrl+\)，就会将信号发送给
前台进程组的所有进程。
 */

/* 保证程序能读写控制终端的方法是打开 /dev/tty。 */
pid_t tcgetpgrp(int filedes);

int tcsetpgrp(int filedes, pid_t pgrpid);
/* 将前台进程组ID设置为pgrpid
 * pgrpid的值应当是在同一会话中的一个进程组的ID
 *
 * filedes必须引用该会话的控制终端。*/

pid_t tcgetsid(int filedes)；                   /* 会话首进程的进程组ID */

/* POSIX.1将孤儿进程组定义为：该组中每个成员的父进程要么是该组的一个成员，要么
 * 不是该组会话所属的成员。
 * 另一种描述为：一个进程组不是孤儿进程组的条件是，该组中有一个进程，其父进程
 * 在属于同一会话的另一个组中。
 * 如果进程组不是孤儿进程组，那么在属于同一会话的另一个组中的父进程就有机会重
 * 启该组中停止的进程。
 * 如果进程组中每一个进程的父进程都属于另一个会话，那么它就是孤儿进程组。*/


/*-----------------------------------------------------------------------------
 * POSIX.1要求向新的孤儿进程组中处于停止状态(例如SIGTSTP信号默认处理）的每一个
 * 进程发送挂断信号（SIGHUP），接着又向其发送SIHCONT信号。对挂断信号的系统默认
 * 动作是终止该进程，为此必须提供一个信号处理程序以捕捉该信号。
 *-----------------------------------------------------------------------------*/
