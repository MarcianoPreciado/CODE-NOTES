/* 第14章：高级I/O
 */

/* 记录锁的功能是：当一个进程正在读或修改文件的某个部份时，它可以组它可以组它
 *它可以阻止其他进程修改同一文件区。UNIX系统内核根本没有文件记录这种概念。更适
 合的术语是字节范围锁（byte-range locking)。
 */


#include	<fcntl.h>

int fcntl(int filedes, int cmd, ... /* struct flock *flockptr */);
struct flock {
        short l_type;  /* F_RDLCK, F_WRLCK, F_UNLCK */
        off_t l_start;
        short l_whence;
        off_t l_len;
        pid_t l_pid;  /* only returned with F_GETLK， */
}
/* 如果 l_len为0，则表示锁的区域从其起点至最大可能便宜量为止。
 * 如果一个进程对一个文件区间已经有了一把锁，后来该进程又企图
 * 在同一文件区间再加一把锁，那么新锁将替换老锁。*/

/*
 * F_GETLK，判断由flockptr描述的锁是否会被另一把锁所排斥。如果已有一把锁，则把
 * 现存锁的信息写到flockptr所指向的结构中。如果不存在这种情况，则除了将l_type
 * 设置为F_UNLCK外，flockptr所指向结构中的其他信息不变。
 * 进程不能使用F_GETLK来测试它自己是否持有一把锁。它绝不会报告调用进程自己持有
 * 自己持有的锁。
 *
 * F_SETLK，设置由flockptr所指向的锁。如果与原有锁冲突，fcntl立即出错返回，
 * EACCES或EAGAIN。 也用来解锁（F_UNLCK）。
 *
 * F_SETLKW 阻塞加锁版本。
 */

/*
 * 如果两个进程相互等待对方持有并且锁定的资源时，则两个进程处于死锁状态。
 * 如果一个进程已经控制了文件中的一个加锁区域，然后它又试图对另一个进程控制的
 * 区域加锁，则它就会休眠。这种情况下，有死锁的可能性。
 *
 * 检测到死锁时，内核必须选择一个进程接收出错返回。不一定是哪个进程。
 */

/*
 * 关于记录锁的自动继承和释放有3条规则：
 * 1，当一个进程终止时，它所建立的锁全部释放。 任何时候关闭一个描述符时，该紧
 *    进程通过这一描述符可以引用的文件上的任何一把锁都被释放（这些锁都是该进程设置的）。

 fd1 = open(pathname, ...);
 lockfd1;
 fd2 = open(pathname, ...)
 close(fd2)               关闭fd2后，锁被释放。
 * 2，由fork产生的子进程不继承父进程所设置的锁。
 * 3，在执行exec后，新程序可以继承原执行程序的锁。但是注意：如果对一个文件描述
 * 符设置了close-on-exec标志，那么当作为exec的一部份关闭该文件描述符时，对相应
 * 文件的所有锁都被释放了。
 *
 */

/*
 * 内核必须独立于当前文件偏移量或文件尾端而记住锁。
 *
 * 异步IO，轮询，I/O多路转接（I/O multiplexing）。
 * 多路转接：先构造一张有关描述符的列表。知道这些描述符的一个已准备好进行I/O时
 * ，该函数返回。
 */

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);
// 如果三个字符集指针都为NULL，则select提供了较sleep更精确的计时器
// nfds的意思是最大描述符+1。也可将第一个参数设置为 FD_SETSIZE常量，说明了最大
// 描述符数。
// 异常状态包括：a，在网络连接上到达的带外数据，或者b 在处于数据包模式的位置伪终端上发生了某些状态。
//
// 对于读/写和异常状态，普通文件描述符总是返回准备好。

void FD_CLR(int fd, fd_set *set);
int  FD_ISSET(int fd, fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_ZERO(fd_set *set); /* 声明一个fd_set字符集后，必须用FD_ZERO清除其所有位，然后设置。 */

#include <sys/select.h>

int pselect(int nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask);

(i)    select() uses a timeout that is a struct timeval (with seconds and
                microseconds), while pselect() uses a struct timespec (with seconds and nanoseconds).

        (ii)   select() may update the timeout argument to indicate how much time was left.
        pselect() does not change this argument.

        (iii)  select() has no sigmask argument, and behaves as pselect() called with
        NULL sigmask.

#include	<sys/time.h>
        struct timeval {
                long    tv_sec;         /* seconds */
                long    tv_usec;        /* microseconds */
        };

/* select
 * timeout == NULL 永远等待。如果捕捉到一个信号，就中断，select返回-1。如果所致
 所指定的描述符中的一个已经准备好，就返回就绪的描述符数。
 *
 * timeout->tv_sec == 0 && timeout->tv_usec == 0，完全不等待，测试后立即返回。
 * timeout->tv_sec != 0 || timeout->tv_usec != 0，等待指定时间后立即返回。
 *
 */

struct timespec {
        long    tv_sec;         /* seconds */
        long    tv_nsec;        /* nanoseconds */
};


#include	<poll.h>
int poll(struct pollfd fdarray[], nfds_t nfds, int timeout);
int ppoll(struct pollfd *fds, nfds_t nfds,
          const struct timespec *timeout_ts, const sigset_t *sigmask);
struct pollfd {
        int   fd;         /* file descriptor */
        short events;     /* requested events */
        short revents;    /* returned events */
};



/* POLLIN There is data to read.
 *
 * POLLPRI
 *        There  is  urgent  data  to read
 * 	  不阻塞地可读高优先级数据。
 *        (e.g., out-of-band data on TCP
 *        socket; pseudo-terminal master in packet mode has seen state change
 *        in slave).
 *
 * POLLOUT
 *        Writing now will not block.
 *
 * POLLRDHUP (since Linux 2.6.17)
 *        Stream socket peer closed connection, or shut down writing half of connection.  The _GNU_SOURCE feature test macro  must  be
 *        defined (before including any header files) in order to obtain this definition.
 *
 *
 * POLLRDNORM
 *        Equivalent to POLLIN.
 *
 * POLLRDBAND
 *        Priority band data can be read (generally unused on Linux).
 *
 * POLLWRNORM
 *        Equivalent to POLLOUT.
 *
 * POLLWRBAND
 *        Priority data may be written.
 *
 * 以下三个异常状态测试，即使在events字段中没有指定这三个值，如果相应条件发生
 * ，也在revents中返回它们。
 *
 * POLLERR
 *        Error condition (output only).
 *
 * POLLHUP
 *        Hang up (output only).
 *
 * POLLNVAL
 *        Invalid request: fd not open (output only).描述符不引用一打开文件。
 */

/*
 * poll不更改events成员。
 * timeout == -1 永远等待（阻塞）
 * timeout == 0  不等待
 * timeout > 0   等待timeout毫秒。
 * */
/*
 * 异步IO，Linux请额外参考aio
 * I/O多路复用，Linux请额外参考epoll，
 * 这两方面以后会加到我的blog里
 * http://sd44.is-programmer.com
 */


#include	<sys/uio.h>
ssize_t readv(int filedes, const struct iovec *iov, int iovcnt);
ssize_t writev(int filedes, const struct iovec *iov, int iovcnt);

struct iovec {
        void *iov_base;
        ssize_t iov_len;
}

/*
 * 存储映射I/O（Memory-mapped I/O)
 */

#include	<sys/mman.h>

void *mmap(void *addr, size_t len, int prot, int flag, int filedes, off_t off);
/* 若成功，返回映射区的起始地址，若出错返回MAP_FAILED。
 *
 * addr参数通常设置为0，由系统选择该映射区其实地址。
 * len是映射的字节数
 * off是要映射字节在文件中的偏移量。与addr通常应当是系统虚拟页长度的倍数。一般为0
 * prot参数可指定为 PROT_READ, PROT_WRITE, PROT_EXEC, PROT_NONE任意组合的按位
 * 或，但对指定映射区的保护要求不能超过文件open模式的访问权限。
 *
 * flag参数：
 * MAP_FIXED，返回值必须等于addr,因为不利于移植，一般不用它。
 * MAP_SHARED，指定存储操作修改映射文件，相当与对文件write。
 * MAP_PRIVATE，与MAP_SHARED必须二选一。本标志说明，对映射区的操作导致创建该映
 * 射文件的一个私有副本。一种用途是调试程序。
 *
 * 如果映射区的长度大于文件的长度，对文件大小以外的映射区内容所做的变动不会在
 * 文件中反映出来。于是我们不能用mmap将数据添加到文件中。如果要做到这一点，必
 * 须先家常该文件。
 * 如果访问映射区的某个部份，而在访问时这一部份内容实际上已不存在，则产生
 * SIGBUS信号。
 *
 * 子进程继承存储映射区，exec后的新程序补集成。
 */
int mprotect(void *addr, size_t len, int prot)； /* 更改显存的映射存储区权限 */
int msync(void *addr, size_t len, int flags);   /* 成功返回0，失败返回-1 */
/*        The  flags  argument  may  have  the bits MS_ASYNC, MS_SYNC, and MS_INVALIDATE set.
 *        MS_ASYNC specifies that an update be scheduled, but the call returns immediately.
 *        MS_SYNC asks for an update and waits for it to complete.
 *        MS_INVALIDATE asks to invalidate other  mappings  of the same file
 *        (so that they can be updated with the fresh values just written).
 */

int munmap(void *addr, size_t len);  /*进程终止时或调用munmap解除映射。关闭文件描述符filedes并不解除。 */


