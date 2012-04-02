/* 第四章：格式化函数
 */
#include "hsfmt.h"

char *ChineseMoney(char *sSrc);
/* ChineseMoney("1234567.89") 输出 "壹佰贰拾叁万肆仟伍佰陆拾柒圆捌角玖分" */

char *ChineseUNumber(char *sSrc);
/* ChineseUNumber("123.45")        输出 "一二三点四五"*/

char *ChineseUNumber(char *sSrc);
/* ChineseUNumber("123.45")        输出 "一百二十三点四五"*/

char *KiloDivisionNumber(char *sSrc);
/* KiloDivisionNumber("1234567.89") 输出 "1,234,567.89"
 */

char *Fmtmsg(char *fmt, ...);
/* Fmtmsg不能与sprintf等组合使用，如果需要组合，可以使用MFmtmsg或SFmtmsg。
 * 另外输出信息大于8192会导致内存溢出。
 */

char *MFmtmsg(char *sDest, char *fmt, ...);

char* SChineseMoney(char *sSrc,char* sIdes,int iDessize); /* 安全方式转换大写钱币 */

char* SChineseNumber(char *sSrc,char* sIdes,int iDessize);

char* SChineseUNumber(char *sSrc,char* sIdes,int iDessize);

char* SFmtmsg(char* sIdes,int iDessize,char *fmt, ...);

