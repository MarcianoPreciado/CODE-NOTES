/* 第13章：守护进程
 */

/* 守护进程也称daemon精灵进程。
 *
 * 父进程ID为0的通常是各种内核进程
 * ID 1：init 系统守护进程，负责启动各运行层次特定的系统服务
 * keventd守护进程为在内核中运行计划执行的函数提供进程上下文。
 * kswaped守护进程也称为页面调出守护进程（pageout daemon）。它通过将脏页面以递
 * 低俗写到磁盘上从而使这些页面在需要时仍可回收使用，支持虚拟子系统。
 * bdflush/kupdated将告诉缓存中的数据冲洗到此盘上。
 * 当可用内存达到下限时，bdflush守护进程将脏缓存区从缓冲池（buffer cache）中噢
 中冲洗到磁盘上。每隔一定时间间隔，Kupdated将脏页面冲洗到磁盘上。以便在系统失
 效时减少丢失的数据。
 */

/* 守护进程编程规则
 *
 * 1，umask将文件模式创建屏蔽字设置为0
 * 2，调用fork，使父进程退出(exit)，这样做：第一，如果该守护进程是做为shell命
 * 命令启动，则父进程终止使得shell认为这条命令已经执行完毕。第二，保证了子进程不是进程组长。
 * 3，调用setsid创建新会话（有人建议在此前再次调用一次fork,然后exec，以保证该
 * 守护进程不是会话首进程，从而防止它取得控制终端，或者无论何时打开终端设备都
 * 都支持O_NOCTTY
 * 4，将当前工作目录更改为根目录。打印机假脱机守护进程有时改为他们的SPOOL目录
 * 5，关闭不再需要的文件描述符。(这一点被很多人诟病，认为是第二版作者的话作者
 作者的画蛇添足）
 * 6，某些守护进程打开/dev/null，使其具有文件描述符0, 1, 2。
 */


#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include	<syslog.h>
#include	<fcntl.h>
#include	<sys/resource.h>
#include	<sys/stat.h>

void daemonize(const char *cmd)
{
        int i, fd0, fd1, fd2;
        pid_t pid;
        struct rlimit rl;
        struct sigaction sa;

        umask(0);

        if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
                perror("can't get fileno limit");
        }

        if ((pid = fork()) < 0) {
                perror("fork error");
        } else if (pid != 0) {
                exit(0);
        }

        setsid();

        sa.sa_handler = SIG_IGN;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        /* 父进程终止时，默认向子进程发送SIGHUP信号，此信号默认终止进程，所以此处需要
         * 忽略掉SIGHUP信号 */
        if (sigaction(SIGHUP, &sa, NULL) < 0) {
                perror("can't ignore SIGHUP");
        }

        if ((pid = fork()) < 0) {
                perror("2nd fork error");
        } else if (pid != 0) {
                exit(0);
        }

        if (chdir("/") < 0) {
                perror("chdir error");
        }

        if (rl.rlim_max == RLIM_INFINITY) {
                rl.rlim_max = 1024;
        }
        for (i = 0; i < rl.rlim_max; i++) {
                close(i);
        }

        fd0 = open("/dev/null", O_RDWR);
        fd1 = dup(0);
        fd2 = dup(0);

        openlog(cmd, LOG_CONS, LOG_DAEMON);
        if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
                syslog(LOG_ERR, "unexpected file destriptors %d %d %d", fd1, fd2, fd2);
        }
        exit(1);
}

/* 有三种方法产生日志消息：
 * 1，内核例程调用log函数。任何一个用户进程打开然后读/dev/klogd设备就可设备就
设备就可以读取这些消息。
 * 2，大多数用户进程（守护进程）调用syslog(3)函数以产生日志消息。
 * 3，在此主机上的一个用户进程，或通过TCP/IP网络连接到此主机的其他主机上的一个
 * 用户进程可将日志消息发向UDP端口514。
 */


#include	<sys.log.h>

void openlog(const char *ident, int option, int facility);
void syslog(int priority, const char *format, ...);
/* priority为facility和level的组合
   format参数以及其他参数传至vsprintf函数进行格式化，%m替换成对应于errno值的出错消息字符串。*/
void closelog(void);
int setlogmask(int maskpri);

/*
 *    option
 *        The option argument to openlog() is an OR of any of these:
 *
 *        LOG_CONS       Write directly to system console if there is an error while
 *                       sending to system logger.
 *
 *        LOG_NDELAY     Open the connection immediately (normally,  the  connection
 *                       is opened when the first message is logged).
 *
 *        LOG_NOWAIT     Don't  wait  for child processes that may have been created
 *                       while logging the message.  (The GNU  C  library  does  not
 *                       create  a  child  process,  so this option has no effect on
 *                       Linux.)
 *
 *        LOG_ODELAY     The converse of LOG_NDELAY; opening of  the  connection  is
 *                       delayed  until  syslog()  is called.  (This is the default,
 *                       and need not be specified.)
 *
 *        LOG_PERROR     (Not in POSIX.1-2001.)  Print to stderr as well.
 *
 *        LOG_PID        Include PID with each message.
 *
 *    facility
 *        The facility argument is used to specify what type of program  is  logging
 *        the  message.  This lets the configuration file specify that messages from
 *        different facilities will be handled differently.
 *
 *        LOG_AUTH       security/authorization messages (DEPRECATED  Use  LOG_AUTH-
 *                       PRIV instead)
 *
 *        LOG_AUTHPRIV   security/authorization messages (private)
 *
 *        LOG_CRON       clock daemon (cron and at)
 *
 *        LOG_DAEMON     system daemons without separate facility value
 *
 *        LOG_FTP        ftp daemon
 *
 *        LOG_KERN       kernel  messages  (these  can't be generated from user pro-
 *                       cesses)
 *
 *        LOG_LOCAL0 through LOG_LOCAL7
 *                       reserved for local use
 *
 *        LOG_LPR        line printer subsystem
 *
 *        LOG_MAIL       mail subsystem
 *
 *        LOG_NEWS       USENET news subsystem
 *
 *        LOG_SYSLOG     messages generated internally by syslogd(8)
 *
 *        LOG_USER (default)
 *                       generic user-level messages
 *
 *        LOG_UUCP       UUCP subsystem
 *
 *    level
 *        This determines the importance of the message.  The levels are,  in  order
 *        of decreasing importance:
 *
 *        LOG_EMERG      system is unusable
 *
 *        LOG_ALERT      action must be taken immediately
 *
 *        LOG_CRIT       critical conditions
 *
 *        LOG_ERR        error conditions
 *
 *        LOG_WARNING    warning conditions
 *
 *        LOG_NOTICE     normal, but significant, condition
 *
 *        LOG_INFO       informational message
 *
 *        LOG_DEBUG      debug-level message
 *
 *        The  function  setlogmask(3)  can be used to restrict logging to specified
 *        levels only.
 *
 */

