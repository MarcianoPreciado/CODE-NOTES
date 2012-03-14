/* 第12章：线程控制
 */

#include	<stdio.h>
#include	<pthread.h>

int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);

int pthread_attr_getdetachstate(const pthread_attr_t *restrict attr,
                                int *detachstate);
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
/* PTHREAD_CREAT_DETACHED, PTHREAD_CREATE_JOINABLE */

int pthread_attr_getstack(const pthread_attr_t *restrict attr,
                          void **restrict stackaddr,
                          size_t *restrict stacksize);
int pthread_attr_setstack(pthread_attr_t *attr,
                          void *stackaddr, size_t *stacksize);
/* stackaddr线程属性被定义为栈的内存单元的最低地址，但这并不必然是栈的开始位置
 * 。对于某些处理器来说，栈是从高地址向低地址方向伸展的，那么statcksize就是栈
 * 栈的结尾而不是开始。pthread_attr_getstackaddr和pthread_attr_setstackaddr过时。
 */

int pthread_attr_getstacksize(const pthread_attr_t *restrict attr,
                              size_t *restrict stacksize);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);

/* 线程属性guardsize控制着线程末尾之后用以避免栈溢出的扩展内存的大小，默认设置
 * 默认设置为PAGESIZE个字节。如果对线程属性stackaddr做了修改，系统就会假设我们
 我们会自己管理栈，并使警戒缓冲区机制无效，等同于把guardsize线程属性设为0。
 */
int pthread_attr_getguardsize(const pthread_attr_t *restrict attr,
                              size_t *restrict guardsize);
int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);

int pthread_getconcurrency(void); /* 返回当前并发度，如果操作系统正控制着并发
				     度（即之前没有调用过pthread_setconcureency函数），则返回0 */
int pthread_setconcureency(int level);          /* level为0的话，撤销之前set所产生的作用 */

int pthread_mutexattr_init(pthread_mutexattr_t *attr);
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);

/* 进程共享属性：
 *
 * _SC_THREAD_PROCESS_SHARED传给sysconf检查平台是否支持进程共享这个属性。
 * 在进程中，多个线程可以访问同一个同步对象，默认行为。互斥量属性为PTHREAD_PROCESS_PRIVATE。
 * 相互独立的多个进程可以把同一个内存区域映射到各自独立的地址空间。这时，多个
 多个进程访问共享数据通常也需要同步。如果进程共享互斥量属性设为PTHREAD_PROCESS_SHARED，从多个进程共享的内存区域中分配的互斥量就可以用于这些进程的同步。
 */
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *restrict attr,
                                 int *restrict pshared);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);

/* 互斥量类型属性：
 */

int pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict attr,
                              int *restrict type);
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);

/* PTHREAD_MUTEX_NORMAL
 *
 *        This  type  of  mutex does not detect deadlock. A thread attempting to relock this
 *        mutex without first unlocking it shall deadlock.  Attempting  to  unlock  a  mutex
 *        locked  by  a different thread results in undefined behavior. Attempting to unlock
 *        an unlocked mutex results in undefined behavior.
 *
 * PTHREAD_MUTEX_ERRORCHECK
 *
 *        This type of mutex provides error checking. A thread  attempting  to  relock  this
 *        mutex  without  first unlocking it shall return with an error. A thread attempting
 *        to unlock a mutex which another thread has locked shall return with  an  error.  A
 *        thread attempting to unlock an unlocked mutex shall return with an error.
 *
 * PTHREAD_MUTEX_RECURSIVE
 *
 *        A  thread attempting to relock this mutex without first unlocking it shall succeed
 *        in locking the mutex. The relocking deadlock which can occur with mutexes of  type
 *        PTHREAD_MUTEX_NORMAL  cannot occur with this type of mutex. Multiple locks of this
 *        mutex shall require the same number of unlocks to release the mutex before another
 *        thread  can acquire the mutex. A thread attempting to unlock a mutex which another
 *        thread has locked shall return with an error.  A thread attempting  to  unlock  an
 *        unlocked mutex shall return with an error.
 *        如果需要把现有的单线程接口放到多线程环境中，递归互斥量是非常有用的。
 *        然而由于递归锁的使用需要一定技巧，它只应在没有其他可行方案下使用。
 *
 * PTHREAD_MUTEX_DEFAULT
 *
 *        Attempting to recursively lock a mutex of this type results in undefined behavior.
 *        Attempting to unlock a mutex of this type which was  not  locked  by  the  calling
 *        thread  results  in undefined behavior.  Attempting to unlock a mutex of this type
 *        which is not locked results in undefined behavior. An implementation may map  this
 *        mutex to one of the other mutex types.
 */

int pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);

int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *restrict attr,
                                  int *restrict pshared);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared);

int pthread_condattr_init(pthread_condattr_t *attr);
int pthread_condattr_destroy(pthread_condattr_t *attr);
int pthread_condattr_getpshared(pthread_condattr_t *restrict attr, int *restrict pshared);
int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);


/* 如果一个函数在同一时刻可以被多个线程安全地调用，就称该函数是线程安全的。
 * 如果一个函数对多个线程来说是可重入的，则说这个函数是线程安全的。但并不能但
 * 但并不能说明对信号处理程序来说该函数也是可重入的。如果函数对异步信号处理程
 * 程序的重入是安全地，那么就可以说函数是 异步-信号 安全地。
 */
/* 以线程安全的方式干里FILE对象的方法。 */

int ftrylockfile(FILE *fp);
void flockfile(FILE *fp);
void funlockfile(FILE *fp);

/* 进程中的所有线程都可以访问进程的整个地址空间。除了使用寄存器以外，线程没有
 * 线程没有办法阻止其他线程访问它的数据，线程私有数据也不例外。
 * 虽然底层的实现部份不能阻止这种访问能力，但管理线程私有数据的函数可以提高线
 * 程间的数据独立数据独立性。
 */

int pthread_key_create(pthread_key_t *keyp, void (*destructor)(void *));
/* 创建的键存放在keyp指向的内存单元。这个键可以被进程中的所有线程使用，但每个
 * 线程把这个键与不同私有数据地址相关联。创建新键时，每个线程的数据地址设为NULL。
 * destructor 为析构函数。
 */

int pthread_key_delete(pthread_key_t *key);
/* 取消键与线程私有数据值之间的关联
 * 注意：调用pthread_key_delete并不会激活与key相关联的析构函数*/

pthread_once_t initflag = PTHREAD_ONCE_INIT;  /* initflag必须是全局或静态变量 */
int pthread_once(pthread_once_t *initflag, void (*initfn)(void));

/* 把键和线程私有数据关联起来
 */
void *pthread_getspecific(pthread_key_t key);   /* 返回线程私有数据值 */
int pthread_setspecific(pthread_key_t key, const void *value);

/* 取消选项
 * PTHREAD_CANCEL_ENABLE, PTHREAD_CANCEL_DISABLE，PTHREAD_CANCEL_ASYNCHRONOUS,
 * PTHREAD_CANCEL_DEFERRED。使用异步取消时，线程可在任意时间取消。
 */

int pthread_setcancelstate(int state, int *oldstate);
/* 原子操作。
 * 在默认情况下，线程在取消请求发出后还是继续运行，直到线程到达某个取消点。
 * 取消点是线程检查是否被取消并按照请求进行动作的一个位置。*/

/* 每个线程都有自己的信号屏蔽字，但是信号的处理是进程中所有信号共享的。
 * 这意味着，尽管单个线程可以阻止某些信号，但当线程修改了与某个信号相关的处理
 处理行为之后，所有线程都必须！！ 共享这个处理行为的改变。

 线程必须用pthread_sigmask来阻止信号发送。
 */

#include	<signal.h>

int pthread_sigmask(int how, const sigset_t *restrict set,
                    sigset_t *restrict oset);       /* 失败时返回错误码，而sigprocmask设置errno返回-1 */
int sigwait(const sigset_t *restrict set, int *restrict signop);
/* set参数指定了线程等待的信号集，signop指向的整数将作为返回值，表明接收到的信号值
 * 如果信号集中的某个信号在sigwait调用时处于未决状态，那么sigwait将无阻塞返回
 * ，返回之前，sigwait将从进程中移除那些处于未决状态的信号。且恢复之前的信号屏
信号屏蔽字。
 *
 * 为避免错误动作，线程在调用sigwait之前，必须阻塞那些它正在等待的信号。
 *
 * 如果多个线程调用sigwait等待同一信号，则线程阻塞，只有一个线程可以从sigwait返回。
 * 如果信号被捕获（例如进程使用sigaction建立了一个信号处理程序），而且线程在
 * sigwait调用中等待同一信号，则未定义以哪种方式递送信号。但不能出现两者皆可的
 * 情况。
 */

int pthread_kill(pthread_t thread, int signo);

/* 要清除锁状态，可通过调用pthread_atfork函数建立fork处理程序。*/

int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void));
// prepare处理程序由父进程在fork子进程前调用，任务是获取父进程定义的所有锁。
//
// parent处理程序是在fork子进程以后，但在fork返回之前在父进程环境中调用，这个
// fork处理程序的任务是对prepare fork处理程序获得的所有锁进行解锁。
//
// child fork处理程序在fork返回之前在子进程环境中调用，也必须释放prepare fork处
// 处理程序获得的所有锁。
//
// 多次调用pthread_atfork可设置多套fork处理程序。此时，
// parent和child fork处理程序是以他们注册时的顺序进行调用的。而parent fork处理
// 处理程序的调用顺序与注册时的顺序相反。

