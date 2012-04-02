/* 第九章：GAPS数据库动态操作函数组（INFORMIX）
 */
/* sSql         SQL语句
 * iResult      影响数据集记录条数（输出：更新笔数/删除笔数） */
IRESULT sql_DynExec(char *sSql, int *iResult);

/* 从指定Load格式文件中提取记录插入到指定数据库表
 * 输入参数: 
 *          char *sFilename       数据文件名(全路径)
 *          char *sTname          表名
 *          char cDiv             分隔符      缺省为'|'
 *          char cEscape          转义字符    缺省为'\'
 *          long *lResult         插入记录条数(输出)
 * 返    回: 
 *          成功
 *            MID_SYS_SUCC
 *          失败
 *            MID_SYS_INVALID
 *            MID_SYS_OPENFILE
 *            
 *            MID_SQL_PREPARE
 *            MID_SQL_DESCRIBE
 *            
 *            MID_SQL_ALLOCDESCPT
 *            MID_SQL_SETDESCPT
 */
IRESULT sql_LoadFromFile(char *sFilename, char *sTname, char cDiv, char cEscape, long *lResult);

/* 根据SQL查询结果集生成UNLOAD格式结果文件
 * 输入参数: 
 *          char *sFilename       数据文件名(全路径)
 *          char *sSql            查询SQL语句
 *          char cDiv             分隔符      缺省为'|'
 *          char cEscape          转义字符    缺省为'\'
 *          long *lResult         插入记录条数(输出)
 * 返    回: 
 *          成功
 *            MID_SYS_SUCC
 *          失败
 *            MID_SYS_INVALID
 *            MID_SYS_OPENFILE
 *            MID_SYS_MEMALLOC
 *            
 *            MID_SQL_DESCRIBE
 *            MID_SQL_CUROPEN
 *            MID_SQL_CURFETCH
 * 
 * 限    制:
 *     最大行长 8K
 */
IRESULT sql_UnloadToFile(char *sFilename, char *sSql, char cDiv, char cEscape, long *lResult);

#include "gaps_sql.h"

/* 获取数据库长整型日期值格式化字符串
 * 输入参数: 
 *          int iDate           长整性日期值
 *          char *sFmt           日期字符串格式  若为空，则作为"yyyymmdd"
 *          char *sDate          输出日期字符串缓冲区
 *          int  iSize           输出缓冲区长度
 * 返    回: 
 *          成功
 *            MID_SYS_SUCC
 *          失败
 *            MID_SYS_FAIL
 * 注意：   sDate应大于sFmt长度
 */
IRESULT sql_GetFmtSDate(int iDate, char *sFmt, char *sDate, int iSize);

/* 获取数据库长整型日期值
 * 输入参数: 
 *          char *sDate          输入日期字符串(yyyymmdd)
 *          int *iDate          返回长整性日期值
 */
IRESULT sql_GetDate(char *sDate, int *iDate);

char *sql_GetMsg(void);

/* 以下函数出错时同时设置sqlca.sqlcode */
IRESULT sql_ConnectDb(char *sIdbname, char *sIdbuser, char *sIdbpwd);

IRESULT sql_ConnectMDb(char *sIdbname,char *sIdbuser,char *sIdbpwd,char *sIconnname); /* conname为连接名称 */

IRESULT sql_SetCurDbconn(char *sIconnname);

IRESULT sql_DisconnDb();
