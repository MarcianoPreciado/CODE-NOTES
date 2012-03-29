第11章：线程

/* POSIX线程的测试宏是_POSIX_THREADS，也可以把_SC_THREADS常数用于调用syconf汉
 *函数，从而在运行时确定是否支持线程。*/

#include	<pthread.h>
int pthread_equal(pthread_t tid1, pthread_t tid2); /* 不想等的话返回0值 */
pthread_t pthread_self(void);

/* 在POSIX线程的情况下，程序开始运行时，它也是以单进程中的单个控制线程启动的。
 */

int pthread_creat(pthread_t *restrict tidp, const pthread_attr_t *restrict attr,
              void *(*start_rtn)(void *), void *restrict arg); /* void *，可传结构 */
/* 新创建的线程可以访问进程的地址空间，并且继承调用线程的浮点环境和信号屏蔽字，
 * 但是该线程的未决信号集将被清除。
 * 注意：pthread函数在调用失败时通常会返回错误码。
 * 每个线程都提供errno的副本，这只是为了与使用errno的现有函数兼容。
 *
 * 注意：pthread函数在调用失败时通常会返回错误码，它们并不像其他POSIX函数一样
 * 设置errno。
 */

/* 单个线程在不终止进程（exit）的情况下，可有3种办法停止它的控制流，退出：
 * 1，线程只是从启动例程中返回，返回值是线程的退出码。
 * 2，线程可以被同一进程中的其他线程取消。
 * 3，线程调用pthread_exit。
 */
void pthread_exit(void *rval_ptr);        /* rval为无类型指针。 */

int pthread_join(pthread_t thread, void **rval_ptr);
/* 调用线程将一直阻塞，直到指定的线程调用pthread_exit，从启动例程中返回或者被取消。
 * 如果线程被取消，用rval_ptr指定的内存单元就被置为PTHREAD_CANCELED.
 * 函数调用成功则返回0，否则返回错误编号。
 *
 * 例如线程函数 return (void *)1, pthread_exit((void *)2);
 * 其他线程可以调用int *tret, pthread_join(&(tret))获取已终止线程的退出码，可
 * 用printf (int)tret打印出来。也可以返回结构，但注意结构使用的内存在调用者完
 * 成调用以后必须仍然是有效的。
 */
int pthread_cancel(pthread_t tid);              /* 请求取消同一进程中的其他线程。 */

void pthread_cleanup_push(void (*rtn)(void *), void *arg);
void pthread_cleanup_pop(int execute);
/* 如果把execute参数设置为0，清理函数将不被调用。
 * 无论何种情况，pthread_cleanup_pop都将删除上次push调用建立的清理处理程序。
 * 这里，APUE中文版中介绍的不全，当调用pthread_exit或其他线程调用
 * pthread_cancel取消该进程时，pthread_clennup_pop才自动执行清理函数。
 */

int pthread_detach(pthread_t tid);               /* 使线程进入分离状态 */

/* 在设计时需要规定所有的线程必须遵守相同的数据访问规则，只有这样，互斥机制才
 *才能正常工作。

 * 以下函数成功返回0，失败返回错误编号。
 *
 * PTHREAD_MUTEX_INTIALIZER 常量，只对静态分配的互斥量。
 */
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const
                       pthread_mutexattr_t *restrict attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutext_t *mutex);
int pthread_mutex_trylock(pthread_mutext_t *mutex);
int pthread_mutex_unlock(pthread_mutext_t *mutex);

/* 死锁原因：对同一个互斥量加锁两次，
 * 或两个线程都在相互请求对另一个线程拥有的加锁资源等等。
 *
 * 一个线程试图以与另一个线程相反的顺序锁住互斥量，才能出现死锁。
 * 如果锁的粒度太粗，就会出现很多线程阻塞等待相同的锁，源自并发性的改善微乎其微。
 * 如果锁得粒度太细，那么过多的锁开销会使系统性能受到影响。
 *
 * 一次只有一个线程可以占有写模式的读写锁，但是多个线程可以同时占有读模式的读写锁。
 * 当读写锁在读加锁状态时，如果线程希望以写模式对此锁进行加锁，它必须阻塞直到
 * 所有的线程释放该锁。
 * 实际实现中，如果有写锁请求，读写锁通常会阻塞随后的读模式锁请求。
 */

int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
                        const pthread_rwlockattr_t *restrict attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
/* 在实现读写锁的时候可能会对共享模式下可获取的锁的数量进行限制，所以需要检查
 * pthread_rwlock_rdlock的返回值。
 */

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock); /* 成功返回0,否则EBUSY */

/* 条件变量给多个线程提供了一个会合的场所。
 * 条件变量与互斥量一起使用时，允许线程以无竞争的方式等待特定的条件发生。
 * 线程在改变条件状态前必须首先锁住互斥量，其他线程在获得互斥量之前不会察觉到
 * 这种改变。
 *
 * 可以把常量PTHREAD_COND_INTIALIZER赋给静态分配的条件变量。
 */

int pthread_cond_init(pthread_cont_t *restrict cond,
                      pthread_condattr_t *restrict attr);

int pthread_cond_destroy(pthread_cond_t *cond);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  pthread_cond_wait
 *  Description:  把调用线程放到等待条件的线程列表上，然后对互斥量进行解锁，这
 *  两个操作是原子操作。
 *  pthread_cond_wait返回时，互斥量再次被锁住。
 * =====================================================================================
 */
int pthread_cond_wait(pthread_cond_t *restrict cond,
                      pthread_mutex_t *restrict mutex);

int pthread_cond_timedwait(pthread_cond_t *restrict cond,
                           pthread_mutex_t *restrict mutex,
                           const struct timespec *restrict timeout);

struct timespec {
        time_t tv_sec;
        long tv_nsec;
}               /* 使用这个结构时，时间值是一个绝对数而不是一个相对数 */

int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
/* 先unlock再broadcast，还是先broadcast再unlock要根据具体情况而定。
 * 第一种，程序可以在unlock和broadcast之间获取互斥锁，然后使条件失效。最后释放
 * 互斥锁。接着当调用pthread_cond_broadcast时，条件不再为真，线程无需运行。
 * 第二种，广播后，等待线程会被调度以运行。如果程序运行在多处理机上，由于还持
 还持有互斥锁，一些线程就会运行而且马上阻塞。
 */

