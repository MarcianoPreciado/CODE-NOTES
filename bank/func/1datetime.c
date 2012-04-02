通用库

第一章：日期时间函数
//~MBP/libincl/common/hsdatetime.h
#include <hsdatetime.h>

char *GetFmtSysDate(char *sFmt);
GetFmtSysDate("%Y 年%m 月%d 日,%H 时%M 分%S 秒,周%u,第%j 天")

char *ConvertFmtDate(char *sSrc, char *sFmt);
ConvertFmtDate("20010531","%Y 年%m 月%d 日")

char *ConvertFmtTime(char *sSrc, char *sFmt);
ConvertFmtTime("131415","%H:%M:%S")

char *GetSysDate(void);
char *GetSysTime(void);


char *CommonDateConvert(char* sSrc,char* sSrcFmt,char* sDestFmt);
CommonDateConvert("2001-05-31 22:23:24","%Y-%m-%d %H:%M:%S","%Y%m%d%H%M%S");
//输出:  "20020531222324"


