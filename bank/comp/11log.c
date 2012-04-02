/* 第十一章：日志记录宏
 */
#include "gaps_sqllog.h"
SQLERRLOG(msglevel,mainmsg,secmsg);
//判断 SQLCODE ,若发生数据库错误则记录 SQL 信息
SQLERRLOGRET(ret,msglevel,mainmsg,secmsg);
//判断 SQLCODE ,若发生数据库错误则记录 SQL 信息并返回 ret 值,并退出所在函数
SQLNOTFOUNDLOG(msglevel,mainmsg,secmsg);
//判断 SQLCODE,若发生无查询结果则记录 SQL 信息
SQLNOTFOUNDRET(ret,msglevel,mainmsg,secmsg);
//判断 SQLCODE ,若发生无查询结果则记录 SQL 信息并返回 ret 值,并退出所在函数
SQLNUPDATELOG(msglevel,mainmsg,secmsg);
//判断 SQLCODE,若发生未成功更新记录则记录 SQL 信息
SQLNUPDATELOGRET(ret,msglevel,mainmsg,secmsg);
//判断 SQLCODE,若发生未成功更新记录则记录 SQL 信息并返回 ret 值,并退出所在函数

#include "gaps_log.h"
LOG(msglevel, mainmsg, secmsg);
LOGRET(ret, msglevel, mainmsg, secmsg);
DATALOG(msglevel, data, datalen);
TRANSLOG(msglevel, mainmsg, secmsg);
TRANSLOGRET(ret, msglevel, mainmsg, secmsg);

