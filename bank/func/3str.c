/* 第三章：字符串操作类
 */
                  
#include <hsstring.h>
char *alltrim(char *str);
/*     alltrim("  abc def gh ");
 *     输出:"abc def gh"
 */

char *trim_n(char *str);      /* 去除表达式中的"|n"字符串 */
char *trim(char *str);        /* 删除字符串中所有空格 */
char *rtrim(char *str);
char *ltrim(char *str);
char *tabtrim(char *str);     /* 删除字符串中所有空格和TAB */

char *StrReplace(char *sSrc, char *sSubOld, char *sSubNew); 
/* 如果出错返回NULL，如果正常则返回替换后的字符串，
 * 注意：影响原字符串 */

char *FilterCR(char *str);    /* 过滤回车 */
char *FilterRemark(char *src, char *dest); /* 将去除注释内容的串放入dest */

char *itoa(int iInt);
char *ltoa(long iLong);

int SubString(char *sSrc, int iStart, int iLen, char *Dest, int iDestlen);

/* 输入参数:
 *           char* sSource  源字符串
 *           int iStart     开始位移(若开始位置大于原始串长,返回NULL)
 *           int iLen       提取长度(若提取长度超过串长,截取至源串结束)
 *           char* sDest    目的字符串
 *           int iDestle    目的字符串长度
 * 函数返回:
 *           int
 *           >0    获取长度
 *           -1    指针为空
 *           -2    起始超长
 */

int pstrcopy(char* sDes,char* sSrc,int iSize);/* 安全拷贝 */

int GetListbyDiv(char *sFrom,int iListno,char *sTo,char cDiv,char cEscape);
/* 函数功能:
 * 取以指定单分隔符分隔的字符串中指定序号对应的域内容(以分隔符cDiv分隔)
 * 输入参数: 
 *   char *sFrom    原始串
 *   int  iListno   分隔域序号 
 *   char *sTo      输出域内容
 *   char cDiv      分隔符
 *   char cEscape   转义符
 * 返    回: 
 *   >0   成功
 *   -1  无指定序号域
 *   -2  参数错误
 */

int AscToHex( char *szHexBuf, int size, char *szAsc, int num );
int HexToAsc( char *szAscBuf, int size, char *szHex, int num );

char *strtoupper(char *str);
char *strtolower(char *str);
