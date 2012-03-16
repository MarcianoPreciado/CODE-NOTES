#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

extern char **environ;

pthread_mutex_t env_mutex;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void
thread_init(void)
{
        pthread_mutexattr_t attr;

        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&env_mutex, &attr);
        pthread_mutexattr_destroy(&attr);
}

int
getenv_r(const char *name, char *buf, int buflen)
{
        int i, len, olen;

        pthread_once(&init_done, thread_init);
        len = strlen(name);
        pthread_mutex_lock(&env_mutex);
        for (i = 0; environ[i] != NULL; i++) {
                if ((strncmp(name, environ[i], len) == 0) &&
                    (environ[i][len] == '=')) {
                        olen = strlen(&environ[i][len+1]);
                        if (olen >= buflen) {
                                pthread_mutex_unlock(&env_mutex);
                                return(ENOSPC);
                        }
                        strcpy(buf, &environ[i][len+1]);
                        pthread_mutex_unlock(&env_mutex);
                        return(0);
                }
        }
        pthread_mutex_unlock(&env_mutex);
        return(ENOENT);
}
/*
* 要使getenv_r可重入，需要改变接口，调用者必须提供自己的缓冲区，避免其他线程的
* 干扰，但是这样还不足以使getenv_r成为线程安全地。还需要在搜索请求的字符串时保
* 护环境不被修改。我们可以使用互斥量，通过getenv_r和putenv函数对环境列表的访问
* 进行序列化。
*/
