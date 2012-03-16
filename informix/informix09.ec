/* 与连接有关的环境变量 */

INFORMIXCONTIME
客户端企图连接服务器的最小秒数。

INFORMIXCONRETRY
尝试链接的次数。

INFORMIXSQLHOSTS
UNIX系统上，这个变量指向sqlhosts文件。
WIN系统上，这是网络上服务器的主机名。

INFORMIXSERVER
默认的INFORMIX服务器。
当客户端程序没有显式指定INFORMIXSERVER环境变量时，自动链接到此服务器。

如果第一条SQL语句是connect语句（CONNECT，SET  CONNECT），程序根据环境变量
INFORMIXSERVER或者指定语句发起显式连接。
如果是SQL database语句（DATABASE, CREATE DATABASE, START DATABASE），程序发起
隐式连接。

/* 中断SQL请求 */
sqlbreak()库函数可中断数据库服务器，当你想中止当前SQL请求或者请求时间过长时。
或者在较大的查询中，设定SIGNAL（SIG_INT, func_ptr)函数，使ctrl+c时中断。
当数据库在一个大的查询中，如果你想周期性的获得何时可以继续请求的话，可以使用
sqlbreakcallback()函数来确定以下信息：
1，timeout interval，在应用程序恢复控制前等待SQL请求执行的周期时间。
2，callback function，用户自定义的每次超过周期时执行的函数。

/* 不要在使用olipcshm作为网络类型的ESQL/C服务器中使用sqlbreakcallback()函数。
 * onipcshm不是一个真正的网络协议，并且并不采用callback function所需要的非阻塞
 * I/O。当你对共享内存使用sqlbreakcallback()时，总是返回真值0，但callbackfunc
 * 并不执行。
 */
void callbackfunc(mint status);
/*  callback合适被调用              callback参数值
 *  数据库服务器完成SQL请求时       0
 *  在程序对server发出请求后立即    1
 *  数据库服务器处理SQL请求超时后去 2
 */
callback可以是下列函数之一：
1， sqldone()，探测数据库是否仍然繁忙。如果返回-439，则繁忙，你可以做终断。
2， sqlbreak()，中断正在执行的SQL服务器操作。


sqlexit()库函数关闭所有连接，包括显式的或者隐式的。如果在数据库打开时调用
sqlexit()，这个函数会导致open transactions回滚。
sqldetach()库函数关闭连接到数据库服务器的子进程。

Informix ESQL/C 提供了'optimized message transfers'功能，允许你对大多数ESQL/C
语句做更小的信息传递，具体实现是，将信息捆绑在一起发送，避免多个小信息包发送发送。
但对以下语句ESQL/C并不进行OPTMSG：

COMMIT WORK
DESCRIBE
EXECUTE
FETCH
FLUSH
PREPARE
PUT
ROLLBACK WORK
SELECT INTO (singleton SELECT)

为使用OPTMSG，必须在客户端设置如下变量：

1，在发起连接前设置OPTMSG环境变量从而允许Optimized Message Transfers
putenv("OPTMSG=1"); /* UNIX */
ifx_putenv("OPTMSG=1"); /* WIN */

2，在ESQL/C应用程序中设置OptMsg全局变量
设置OPTMSG环境变量为1后，必须设置OptMsg全局变量用以指定后面的SQL语句捆绑。
不过当一些SQL语句需要立即被回应，或者在DEBUGGING，或者程序即将结束时，最好禁用。
EXEC SQL include sqlhdr;
OptMsg = 1;

当OPTMSG被启用时，ESQL/C应用程序不能对捆绑语句中的某条做准确的异常处理。
一旦在捆绑语句段中有错误发生，数据库服务器停止执行。

