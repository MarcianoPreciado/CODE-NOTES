/* 如果守护进程调用chroot,须在调用chroot之前调用选项为LOG_NDELAY的openlog。它
 *它打开特殊设备文件并生成描述符，FTPD这样的守护进程中经常见到。
 */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  FILE.PID
 *  Description:  保证只运行某守护进程的一个副本
 * =====================================================================================
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<syslog.h>
#include	<errno.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<unistd.h>

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int lockfile(int fd)
{
        struct flock fl;

        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        return fcntl(fd, F_SETLK, &fl);
}

int already_running(void)
{
        int fd;
        char buf[16];

        fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);
        if (fd < 0) {
                syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
                exit(1);
        }

        if (lockfile(fd) < 0) {
                if ( errno == EACCES || errno == EAGAIN) { /* 如果已经加锁 */
                        close(fd);
                        return(1);
                }
                syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
                exit(1);
        }

        ftruncate(fd, 0); /* 注意，这里才截短为0 */
        sprintf(buf, "%ld", (long)getpid());
        write(fd, buf, strlen(buf) + 1);
        return 0;
}


/*
 * ===  FUNCTION  ======================================================================
 *  Description:  守护进程重读配置文件（使用已被屏蔽的SIGHUP信号）。
 * =====================================================================================
 */
void
reread (void)
{
}

void sigterm(int signo)
{
        syslog(LOG_INFO, "got SIGTERM; exiting");
        exit(0);
}

void sighup(int signo)
{
        syslog(LOG_iNFO, "Re-reading config file");
        reread();
}

int main(int argc, char *argv[])
{
        char *cmd;
        struct sigaction sa;

        if (cmd = strrchr(argv[0], '/') == NULL) {
                cmd = argv[0];
        } else {
                cmd++;
        }

        daemonize(cmd);

        if (already_running()) {
                syslog(LOG_ERR, "daemon already running");
                exit(1);
        }

        sa.sa_handler = sigterm;
        sigemptyset(&sa.sa_mask);
        sigaddset(&sa.sa_mask, SIGHUP);
        sa.sa_flags = 0;
        if (sigaction(SIGTERM, &sa, NULL) < 0) {
                syslog(LOG_ERR, "can't catch SIGTERM: %s", strerror(errno));
                exit(1);
        }

        sa.sa_handler = sighup;
        sigemptyset(&sa.sa_mask);
        sigaddset(&sa.sa_mask, SIGTERM);
        sa.sa_flags = 0;
        if (sigaction(SIGHUP, &sa, NULL) < ) {
                syslog(LOG_ERR, "can't catch SIGHUP: %s", streeror(errno));
                exit(1);
        }

        /*  proceed with the rest of the daemon
         */

        return 0;
}
