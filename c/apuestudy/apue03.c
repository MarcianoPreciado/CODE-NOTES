/* 第3章 文件I/O
 */

#include <fcntl.h>

int open(const char *pathname, int oflag,.../* mode_t mode */);
O_RDONLY
O_WRONLY
O_RDWR

O_APPEND
O_CREAT                /* 使用此选项时，需使用mode参数指定访问权限位 */
O_EXCL
O_TRUNC
O_NOCTTY
O_NONBLOCK
O_SYNC

/* creat的不足之处在于它以只写方式打开所创建的文件。若要先写再读该文件，必须先creat, close再调用open。另外creat函数在<fcntl.h>头文件中定义 */

#include <unistd.h>
off_t lseek(int filedes, off_t offset, int whence);

/* 检测lseek返回时是否出错，应当检测其是否为 -1，因为有的系统（freebsd)lseek
   返回值off_t可能是负值任何一个需要调用多个函数的操作都不可能是原子操作，因
   为在两个调用函数之间，内核有可能会临时挂起该进程。*/

/* 内核使用三种数据结构标识打开的文件：
 *
 * 1,每个进程在进程表中都有一个记录项。其中包含一张打开文件描述符表。
 * 文件描述符表包括文件描述符标志close on exec。
 * 指向同一个文件表项的指针。
 *
 * 2,内核为所有打开文件维持一张文件表。包含：
 * (a)文件状态标志（读写/添加/同步和非阻塞）
 * (b)当前文件偏移量
 * (c)指向该文件v节点表项的指针
 *
 * 3,每个打开文件都有一个v节点结构。v节点包含了文件类型和对此文件进行各种操作
 * 的函数的指针。对于大多数文件，v节点还包含了该文件的i结点（索引节点）。Linux
 * 中没有使用v结点，使用了通用i结点，一个独立于文件系统的i结点和一个依赖于文件
 * 系统的i结点。
 */

/*
 * 原子操作：
 * SUS包括了XSI扩展，该扩展允许原子性地定位搜索和执行IO。
 */

ssize_t pread(int filedes, void *buf, size_t nbytes, off_t offset);
ssize_t pwrite(int filedes, void *buf, size_t nbytes, off_t offset);

int dup(filedes);                    /* 总是返回当前可用文件描述符中的最小值 */
int dup2(int filedes, int filedes2);
/*
 * 两个文件描述符指向同一个文件表项，共享同一文件状态标志以及文件偏移量。
 * 但每个文件描述符都有一套文件描述符标志。新描述符的执行时关闭(close-on-exec)
 * 标志总是由dup函数清除。文件描述符标志与文件状态标志有作用域的区别。前者只用
 * 于一个进程的一个文件描述符，而后者则适用于指向该给定文件表项的任何进程中的
 * 所有描述符。
 */

int fsync(int filedes);      /* 等待写磁盘操作结束，然后返回，确保修改过的块立即写到磁盘上 */
int fdatasync(int filedes);  /* 只影响文件数据部份，但fsync同步更新文件属性 */
void sync(void);             /* 只是将所有修改过的块缓冲区排入写队列，然后就返
                                回，不等待实际写磁盘操作结束 */
/*
 * fcntl函数有5种功能
 *  1，复制一个现有文件描述符 	F_DUPFD
 *  2，获得/设置文件描述符标记 	F_GETFD F_SETFD
 *  3，获得/设置文件状态标志 	F_GETFL F_SETFL
 *  4，获得/设置异步I/O所有权 	F_GETOWN F_SETOWN     接收SIGIO和SIGURG信号的
 *  进程ID（正数）或进程组ID（负数）, F_GETOWN返回值为一个正的进程ID或负的进程
 *  组ID
 *  5，获得/设置记录锁 		F_GETLK F_SETLK F_SETLKW
 */

// 文件状态标志里的O_RDONLY O_WRONLY O_RDWR互斥，可用O_ACCMODE获取当前读写状态


