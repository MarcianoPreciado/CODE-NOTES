/* 第15章：进程间通信
 */

/* 1，管道
 * 历史上，管道是半双工的。他们只能在具有公共祖先的进程之间使用。可以用
 * S_ISFIFO宏来测试是否为管道。
 * 单个进程中的管道几乎没有任何用处。通常，调用pipe的进程接着调用fork。这样就
 * 创建了从父进程到子进程的IPC通道。
 *
 * 当管道的一端被关闭后，两条规则起作用：
 * (1)，当读一个写端已被关闭的管道时，在所有数据都被读取后，read返回0，以指示
 * 到达了文件结束处。
 * (2)，如果写一个读端已被关闭的管道，则产生信号SIGPIPE。如果忽略该信号或者捕
 * 捉该信号，则write返回-1，errno设置为EPIPE。
 *
 * 常量PIPE_BUF规定了内核中管道缓冲区的大小。如果对管道调用write且要求写的字节
 * 数小于PIPE_BUF，则此操作不会与其他进程对同一管道的write操作穿插执行，反之则
 * 未必。
 */

#include	<unistd.h>
int pipe(int filedes[2]);

close(fd[1]);
if (fd[0] != STDIN_FILENO)
{
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
}


#include	<stdio.h>
FILE *popen(const char *cmdstring, const char *type);
/* 若成功则返回文件指针，失败返回NULL。如果type是"r"，则文件指针连接到cmd的比
 * 标准输出。如果type是"w"，则文件指针连接到cmd的标准输入。 */

int pclose(FILE *fp);      /* 返回cmd的终止状态，若出错则返回-1 */
/* shell命令 ${PAGER:-more}的意思是，如果shell变量PAGER已经定义，且其值飞快噢
非空，则用其值，非则使用字符串"more"。

 * 在pclose实现中。若pclose的调用者已经为信号SIGCHLD设置了一个信号处理程序，则
 * pclose中的waitpid调用将返回一个EINTR。因为允许调用者捕捉此信号（或者任何期
 * 任何其他可能中断waitpid调用的信号），所以当waitpid被一个捕捉到的信号中断时，
 * 我们只是再次调用waitpid。
 *
 * pclose特别适用于构造简单的过滤器程序，它变换运行命令的输入或输出。
 *
 */

/*
 * 当一个程序产生某个过滤程序的输入，同时又读取该过滤程序的输出时，则改过滤程
过滤程序就成为协同进程（coprocess）。可创建两个Pipe管道来实现。
 */

#include	<sys/stat.h>
int mkfifo(const char *pathname, mode_t mode);
/*
 * 在一般情况下（没有指定O_NONBLOCK），只读open要阻塞到某个其他进程为写二打开
而打开此FIFO，只写open要阻塞到某个进程为读而打开它。

 * 如果指定了O_NONBLOCK，则只读open立即返回。但是如果没有进程已经为读而打开一
一个FIFO，那么只写open将出错返回-1，其errno是ENXIO。
 * 这样一来，如果我们要用非阻塞方式为读写而打开FIFO，则需要：
 */

unlink(fifo);
if (mkfifo(fifo, 0644) < 0)
{
        err_sys("mkfifo error");
}
if ((fdread = open(fifo, O_RDONLY | O_NONBLOCK)) < 0) /* 非阻塞打开open */
{
        err_sys("error rdopen");
}
if ((fdwrite = open(fifo, O_WRONLY)) < 0)       /* 阻塞write。如果阻塞打开open，则open write出错 */
{
        err_sys("error write");
}
clr_fl(fdread, O_NONBLOCK);                     /* 清除关闭读描述符的非阻塞 */


/*
* 类似于管道，若用write写一个尚无进程为读而打开的FIFO，则产生信号SIGPIPE。若
* 某个FIFO的最后一个写进程关闭了该FIFO，则将为该FIFO的读进程产生一个文件结束
* 文件结束标志。
*
* 常量PIPE_BUF说明了可被原子地写到FIFO的最大数据量。
*/

