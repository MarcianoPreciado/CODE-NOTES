/* 第7章：进程环境
 */

/* 有8种方式使进程终止(termination)，其中5种为正常终止，他们是
 * 1，从main返回 return
 * 2，调用exit
 * 3, 调用_exit或_Exit  ISO C定义_Exit，POSIX.1说明_exit，UNIX系统中两者同义
 * 4, 最后一个线程从其启动例程返回  进程终止状态为0，与线程返回值无关。
 * 5，最后一个线程调用pthread_exit  这样进程终止状态总是0。
 * 异常终止有3种方式，他们是
 * 1，调用abort  SIGABRT信号，下一种异常终止的特例
 * 2，接到一个信号并终止
 * 3，最后一个线程对取消请求做出相应。
 *
 * 内核使程序执行的唯一方法是调用一个exec函数。
 * 进程自！！愿！！！终止的唯一方法是显式或隐式的（通过exit）调用_exit或_Exit。
 * 不管进程如何终止，最后都会执行内核中同一段代码。为相应进程关闭所有打开文件
关闭所有打开文件描述符，释放它所使用的存储器等。
*/

#include    <stdlib.h>
int atexit(void (*func)(void));
extern char **environ;

int putenv(char *str);
int setenv(const char *name, const char *value, int rewrite); /* 若rewrite非0，则首先删除其现有定义。若rewrite为0，则不删除其现有定义。 */
/* putenv和setenv差别：
 * Linux和Solaris中，putenv实现将传送给它的字符串地址作为参数直接放入环境表中。
 * 如果字符串存放在栈中，且作为参数，函数返回时，栈销毁，可能造成错误。
 */

#include    <setjmp.h>
int setjmp(jmp_buf env);
/* 若直接调用则返回0，若从longjmp调用返回则返回longjmp参数里的val值 */
void longjmp(jmp_buf env, int val);
/* 声明为全局或静态变量的值在执行longjmp时保持不变。volatile也可。 */

/*
 * 全局、静态和易失变量不受编译器优化的影响。
 * getlimit、setrlimit具体参数请参考manpage。
 *
 * 更改资源限制时，遵循下列三条原则：
 * 1，任何一个进程都可将一个软限制值改为小于或等于其硬限制值。
 * 2，任何一个进程都可以降低其硬限制值，但它必须大于或等于其软限制值。对于普通用户而言这是不可逆的。
 * 3，只有超级用户进程可以提高其硬限制值。
 *
 */
