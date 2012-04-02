#include "log.h"

int SetLogPath(char *path);

void SetPathMode(int iPathMode);  /* 0，直接存储，缺省。1,日期区分目录存储 */

int SetErrLogName(char *name);    /* 错误日志文件名 */

int SetTransLogName(char *name);  /* 传输日志文件名 */

int SetTrackLogName(char *name);  /* 跟踪日志文件名 */

/* progname，错误日志程序名。line，出错文件行号。errormsg，错误信息
 */
int ErrLog(char *progname, int line, char *errormsg); /*  */
int ErrLogF(char *progname, int line, char *fmt, ...);

int InitTransLog();               /* 传输日志文件初始化 */
int CloseTransLog();
/*   记录传输日志
 *   参数: buf    -- 日志信息
 *         len    -- 长度
 *         msg    -- 说明信息
           level  -- 日志级别
 */
int TransLog(char *buf, int len, char *msg, int level);

/* 跟踪系统运行结果
 * 函数参数:   progname -- 记录被跟踪的程序名
 *                 line -- 被跟踪文件的行号
 *                  fmt -- 跟踪信息格式
 */
int TrackLog(char *progname, int len, char *msg);

int TrackLogF(char *progname, int line, char *fmt, ...);

/* 函数功能:记录加长系统错误(可按格式)
 * 函数参数:progname -- 记录错误日志的程序名(一般为__FILE__)
 *          line     -- 出错文件行号(一般为__LINE__)
 *          mode     -- 模式
 *         LOG_NOHEAD  0  无行首信息
 *         LOG_HEAD    1  有行首信息
 *         fmt      -- 错误信息格式
 */
int ExErrLogF(char *progname, int line, int mode, char *fmt, ...);
int ExTrackLogF(char *progname, int line, int mode, char *fmt, ...);
int ExTransLog(char *buf, int len, char *msg, int level, int mode);

/*
 * ===  FUNCTION  ====================================================
 *         Name:  LLOG
 *  Description:  记录加长分级日志
 * ===================================================================
 */
int ErrLLogF(char *progname, int line, int mode,int loglevel, char *fmt, ...);
int TransLLog(char *buf, int len, char *msg, int mode,int loglevel);

