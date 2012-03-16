第4章： 文件和目录

#include	<stat.h>

int stat(char *restrict pathname, struct stat *restrict buf);
int fstat(int filedes, struct stat *buf);
int lstat(const char *restrict pathname, struct stat *restrict buf);

/* lstat类似于stat，但是当命名的文件是一个符号链接时，lastat返回符号链接本身，
 * stat返回该符号链接所引用的文件信息。
 */

//文件类型宏，参数均为struct stat结构中的st_mode成员
//也可用屏蔽字S_IFMT与st_stat进行逻辑与运算
//
S_ISREG()                                       /* 普通文件 */
S_ISDIR()                                       /* 目录文件 */
S_ISCHR()                                       /* 字符特殊文件 */
S_ISBLK()                                       /* 块特殊文件 */
S_ISFIFO()                                      /* 管道或FIFO */
S_ISLNK()                                       /* 符号链接*/
S_ISSOCK()                                      /* 套接字 */
/*
 * 实际用户ID和实际组ID标识我们究竟是谁。
 * 有效用户ID，有效组ID以及附加组ID决定了我们文件的访问权限。
 * 保存的设置用户ID和保存的设置组ID在执行一个程序时包含了有效用户ID和有效组ID的副本。
 *
 * 通常，有效用户ID等于实际用户ID，有效组ID等于实际组ID。
 * 每个文件都有一个所有者st_uid，有一个组所有者st_gid。可以在文件模式字
 * (st_mode)中设置一个标志，含义是“当执行此文件时，将进程的有效用户ID设置为文
 * 件的所有者ID st_uid，称为设置用户ID(set - user - ID)。st_gid类同。这两位可
 * 用常量S_ISUID和S_ISGID测试。
 *
 * 对目录的读权限，可获得在该目录中所有文件名的列表。
 * 如果要在open()中指定O_TRUNC标志，必须对文件有写权限。
 * 为了在一个目录中创建一个新文件或删除一个现有文件，必须对该目录具有写和执行
 * 权限。删除的话，对文件本身不需要有读写权限。
 *
 * 创建新文件时，新文件的用户ID等设置为进程的有效用户ID。
 * 关于组ID，POSIX.1允许下面两种作为新文件的组ID。
 * 1，新文件的组ID可以是进程的有效组ID
 * 2，新文件的组ID可以是它所在目录的组ID，FreeBSD和MAC总是如此。
 * Linux和Solaris下，如果目录的设置组id已经设置，则将新文件的组ID设置为目录的组ID，否则将其设为进程的有效组ID。
 *
 * access函数是按实际用户ID和实际组ID进行访问权限测试的。
 * sys/stat.h 为了改变一个文件的权限位(chmod fchmod)，进程的有效用户ID必须等于文件的所有者ID，或者该进程必须具有超级用户权限。
 *
 */
S_ISUID 	执行时设置用户ID
S_ISGID 	执行时设置组ID
S_ISVTX 	保存正文（粘住位）
chmod fchmod无视umask值。
ls - l中，小写s，代表执行和设置用户（组）权限。大写S，代表非执行和设置用户（组）权限。

int chown(const char *pathname, uid_t owner, gid_t group);
int fchown(int filedes, uid_t owner, gid_t group);
int lchown(const char* pathname, uid_t owner, gid_t group); /* 更改符号链接本身 */
/*
 * 若_POSIX_CHOWN_RESTRICTED常量对指定的文件起作用，则：
 * 1，只有超级用户进程能更改该文件的用户ID。
 * 2，如果进程拥有此文件（其有效用户ID等于该文件的用户ID）
 * 2，参数owner等于 -1或文件的用户ID，并且参数group等于进程的有效组ID或附加组ID。
 * 这意味着，当_POSIX_CHOWN_RESTRICTED起作用时，你不能更改其他用户文件的用户ID。
 * 如果这些函数由非超级用户进程调用，则在成功返回时，该文件的设置用户ID和设置组ID位都会被清除。
 *
 * 在stat结构中，链接计数保存在st_nlink成员中，其基本系统数据类型是nlink_t。这种链接类型是硬件链接。
 *
 */

/* <stdio.h>中的remove函数，对于文件，与unlink相同，对于目录与rmdir功能相同。
 */

ssize_t readlink(const char *restrict pathname, char *restrict buf, size_t bufsize); /* 在buf中返回的符号链接内容不以null字符终止 */

#include 	<utime.h>
int utime(const char *pathname, const struct utimbuf *times);

/* 更改文件的访问和修改时间，
 * 如果times是一个空指针，则访问时间和修改时间两者都设置为当前时间。
 * 执行此操作，需要进程对该文件具有写权限或者进程的有效用户ID等于文件的所有者ID。
 *
 * 如果times是非空指针，需
 * 进程的有效用户ID等于文件的所有者ID，或者进程必须是一个超级用户进程，单单对文件只具有写权限是不够的。
 */
/*
 * 只有内核才能写目录，
 * 每个文件系统所在的存储设备都由其主次设备号表示。主设备号标识设备驱动程序，
 * 有时编码为与其通信的外设板；次设备号标识特定的子设备。在同一磁盘驱动器上的
 * 各文件系统通常具有相同的主设备号，而其次设备号却不同。只有字符特殊文件和块
 * 特殊文件才有st_rdev值。此值包含实际设备的设备号。
 * major minor两个宏来访问主、次设备号。
 */
