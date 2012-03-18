#include    <stdio.h>
#include    <wchar.h>

int fwide(FILE *fp, int mode);
/* 若mode参数值为负，fwide将试图使指定的流是字节定向的。
 * 若mode参数值为正，fwide将试图使指定的流是宽定向的。
 * 若mode参数值为0，fwide将不试图设置流的定向，但返回标识该流定向的值。
 * 如果是宽定向，返回正值，若流是字节定向返回负值，未定向返回0值。
 * fwide无出错返回。可在fwide前清除errno，从fwide返回时再检查errno值。
 */

/* 术语冲洗(flush)说明标准IO缓冲区的写操作。在标准I / O库方面，flush意味着缓冲
 * 区中的内容写到磁盘上。在终端驱动程序方面，flush表示丢弃已存储在缓冲区中的数
 * 据。
 */

/* setbuf, setvbuf这些函数一定要在流已被打开后调用。 */

void setbuf(FILE *restrict fp, char *restrict buf);
int setvbuf(FILE *restrict fp, char *restrict buf, int mode, size_t size);
int fflush(FILE *fp);                           /* 如果出错返回EOF，否则0 */
/* _IOFBF _IOLBF _IONBF。 char *buf最好设为NULL，由标准I / O库自动为该流分配适
 * 当长度的缓冲区。*/

FILE *fopen(const char *restrict pathname, const char *restrict type);

FILE *freopen(const char *restrict pathname, const char *restrict type, FILE *restrict fp);
/* 在一个指定的流上打开一个指定的文件，如若该流已经打开，则先关闭该流。若该流
 * 已经定向，则freopen清除该定向。一般用于标准输入输出等重定向到文件 */

FILE *fdopen(int filedes, const char *type);
/* 因为是文件描述符，此函数常用于由创建管道和网络通信通道函数返回的描述符，因
 * 为这些特殊类型的文件不能用标准I/O fopen函数打开。因为描述符已经打开，所以
 * fdopen不能截短文件。 */

/* 当以读和写类型打开一文件时（type中有"+"号），
 * 如果中间没有fflush、fseek、fsetpos或rewind，则在输出的后面不能直接跟随输入。
 * 如果中间没有fseek、fsetpos或rewind，或者一个输入操作没有到达尾端，则在输入
 * 操作后不能直接跟随输出。
 */

/* "r+"文件必须已经存在，并且不擦除文件以前的内容，默认直接从文件开头开始覆写。
 */

int getc(FILE *fp);
int fgetc(FILE *fp);
/* 两者的区别是，getc可被实现为宏，而fgetc不能实现为宏。这意味着，getc的参数不
 * 应当是具有副作用的表达式。可以得到fgetc的函数指针。如果到达文件结尾或出错则
 * 返回EOF，为区分这种不同，必须调用ferror或feof。
 */

/*  fgets读到换行符为止，但是不超过 N -1 个字符。缓冲区总是以NULL字符结束 */
char *fgets(char *restrict buf, int n, FILE *restrict fp); /* 成功的话返回buf，否则NULL */
char *gets(char *buf);                          /* 不将换行符存入缓冲区 */

int fputs(const char *restrict str, FILE *restrict fp); /* 成功返回正值，否则EOF */
int puts(const char *str);

int ferror(FILE *fp);
int feof(FILE *fp);

void clearerr(FILE *fp)；    /* 清除FILE对象中的出错标志和文件结束标志 */

long ftell(FILE *fp);        /* 成功返回文件当前位置指示，否则返回-1 */
int fseek(FILE *fp, long offset, int whence); /* 成功返回0值，否则返回非0值 */

int fgetpos(FILE *restrict fp, fpos_t *restrict pos);
int fsetpos(FILE *fp, const fpos_t *pos);
/* 注意是指针。。。fgetpos将文件位置指示器的当前值存入pos指向的对象中。在以后
 * 调用fsetpos时，可用此值重新定位至该位置。
 * 成功的话返回的值，出错返回非易值。
 */

char *tmpnam(char *ptr);
/* 每次都产生一个不同路径名，最多调用次数是TMP_MAX，如若ptr不是NULL，则认为它
 * 指向长度至少是L_tmpnam个字符的数组。如果ptr是NULL，则每次调用，都会重写存放
 * 路径名的静态区，所以我们应当保存该路径名的副本。 */

FILE *tmpfile(void);
/* 创建一个临时二进制文件，"wb+"，在关闭该文件或程序结束时，将自动删除这种文件。*/

SUS XSI扩展部份
char *tempnam(const char *directory, const char *prefix);
/* 如果prefix非NULL，它应当是最多包含5个字符的字符串，用其作为文件名的头几个字符。 */

int mkstemp(char *template);
/* 该函数返回临时文件的打开文件描述符，可读写。mkstemp创建的临时文件不会自动被
 * 删除。
 * template是一个路径名，例如 /tmp/tmname.XXXXXX。最后六个字符为XXXXXX。
 * 如mkstemp成功返回会自动修改template字符串以反映临时文件的名字。 */
