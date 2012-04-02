/* 第8章：普通资源处理操作函数组
 */

#include "gaps_pkg.h"
/* 获取ICXP报文配置结构
 * 输入参数：
 *          ICXPCFG *pIcxpcfg       ICXP报文配置结构指针
 *          char *sIResname         报文资源名称
 *  返回：
 *      成功        MID_SYS_SUCC
 *      失败        MID_SYS_INVALID
 *                  MID_SYS_PKGGETCFG   报文资源未找到
 *                  MID_SYS_PKGISEMPTY  报文资源配置为空
 *                  MID_SYS_PKGGETCFG   报文组件：获取报文资源[%s]配置失败
 * */
IRESULT fpub_GetIcxpCfg(HXMLTREE lXmlHandle, ICXPCFG *pIcxpcfg, char *sIResname);
IRESULT fpub_GetXmlpCfg(HXMLTREE lXmlHandle, XMLPCFG *pXmlpcfg, char *sIResname);

IRESULT fpub_GetP8583Cfg(HXMLTREE lXmlHandle, P8583CFG *pP8583cfg, char *sIResname);

IRESULT fpub_GetXmlpRoot(HXMLTREE lXmlHandle, char *sRootname, char *sIResname);

#include "gaps_tstruct.h"

/* 获取TSTRUCT表结构配置结构
 * 输入参数: 
 *          TSTRUCTCFG *pTstructcfg  表结构配置结构指针
 *          char *sColname           表结构资源列名称
 *          RMA_TSTRUCTITEM *pStstructitem 表结构配置项指针
 * 返    回: 
 *          成功
 *          MID_SYS_SUCC
 *          失败
 *          MID_SYS_INVALID        无效参数
 *          MID_SYS_TSTRUCTGETCFGITEM  表结构资源配置未找到
 */
IRESULT fpub_GetTstructCfgItem(TSTRUCTCFG *pTstructcfg, char *sColname, RMA_TSTRUCTITEM *pStstructitem);

/* 获取TSTRUCT表结构配置结构
 * 输入参数: 
 *          HXMLTREE lXmlHandle      RXDA句柄
 *          TSTRUCTCFG *pTstructcfg  表结构配置结构指针
 *          char *sIResname          表结构资源名称
 * 返    回: 
 *          成功
 *          MID_SYS_SUCC
 *          失败
 *          MID_SYS_INVALID        无效参数
 *          MID_SYS_TSTRUCTGETCFG  表结构资源配置未找到
 *          MID_SYS_TSTRUCTISEMPTY 表结构资源配置为空
 *          MID_SYS_TSTRUCTGETCFG  表结构组件：获取表结构资源[%s]配置失败               
 */
IRESULT fpub_GetTstructCfg(HXMLTREE lXmlHandle,
                           TSTRUCTCFG *pTstructcfg,char *sIResname);

/* 函数功能:根据表结构定义，从RXDA数据中动态插入表记录
 * 输入参数: 
 *          HXMLTREE lXmlhandle    RXDA句柄
 *          char *sTname           表名
 *          char *sIResname        表结构名
 *          int  iMode             数据提取模式
 *                                 0   插入第一条记录
 *                                 1   插入最后一条记录
 *                                 2   根据基准元素节点的个数插入相应记录
 *          char* sBaseElem        基准元素名
 */
IRESULT sql_DynInsByRxda(HXMLTREE lXmlhandle, char *sTname, char *sIResname,
                         int iMode, char *sBaseElem);

/* 函数功能:根据表结构定义，根据RXDA数据动态更新表记录
 * 输入参数: 
 * 
 *          HXMLTREE lXmlhandle    RXDA句柄
 *          char *sTname           表名
 *          char *sIResname        表结构名
 *          char *sUpdlist         更新字段列表
 *          char *sWhere           更新条件表达式
 *          int*  iResult          影响数据集记录条数  (输出:更新笔数)
 */

IRESULT sql_DynUpdByRxda(HXMLTREE lXmlhandle,char *sTname,char *sIResname,
                         char *sUpdlist,char *sWhere,int*  iResult);

/* 函数功能:动态执行SQL查询结果集，根据表结构定义存储到RXDA
输入参数: 
         HXMLTREE lXmlHandle   XML句柄
         char *sSql            查询SQL语句
         char *sIResname       表结构名
 *       char *sBaseNode       RXDA基准节点,若为空,则表配置中的相对路径仍为
 *                             相对应用路径,若不为空,则相对sBaseNode/record
 *                             存放,基准节点对绝对路径无效;基准节点本身可以
 *                             为相对应用的相对路径
 *       int *iResult          需要记录条数(输入,若为零则取所有结果集)
 *                             记录条数(输出)
 *       int  iMode            模式  0 附加 1 覆盖
 * 
 * 返    回: 
 *          成功
 *            MID_SYS_SUCC
 *          失败
 *            MID_SYS_FAIL
 *            MID_SYS_INVALID   无效参数
 *            MID_SQL_PREPARE
 *            MAX_QUERY_ROW     记录超限
 *                             
 * 限    制:
 *   最大允许列数
 *   最大允许列长度32K
 *   最大SQL语句长度2K
 */
IRESULT sql_DynSelToRxda(HXMLTREE lXmlHandle, char *sSql, char *sIResname,
                         char *sBaseNode, int *iResult, int iMode); 

#include "gaps_file.h"

/* 获取文件配置结构
 * 输入参数: 
 *          HXMLTREE lXmlHandle      RXDA句柄
 *          FILECFG *pFilecfg        文件配置结构指针
 *          char *sIResname          文件资源名称
 * 返    回: 
 *          成功
 *          MID_SYS_SUCC
 *          失败
 *          MID_SYS_INVALID    无效参数
 *          MID_SYS_PKGGETCFG  文件资源配置未找到
 *          MID_SYS_PKGISEMPTY 文件资源配置为空
 */
IRESULT fpub_GetFileCfg(HXMLTREE lXmlHandle, FILECFG *pFilecfg, char *sIResname);

#include "gaps_mon.h"

/* 返    回: 
 *          成功
 *          MID_SYS_SUCC
 *          失败
 *          MID_SYS_INVALID    无效参数
 *          MID_SYS_MONGETCFG  监控资源配置未找到
 *          MID_SYS_MONISEMPTY 监控资源配置为空
 */
IRESULT fpub_GetMonCfg(HXMLTREE lXmlHandle, MONCFG *pMoncfg, char *sIResname);

#include "gaps_bill.h"

/* 返    回: 
 *          成功
 *          MID_SYS_SUCC
 *          失败
 *          MID_SYS_INVALID     无效参数
 *          MID_SYS_BILLGETCFG  凭单格式资源配置未找到
 *          MID_SYS_BILLISEMPTY 凭单格式资源配置为空
 */
IRESULT fpub_GetBillCfg(HXMLTREE lXmlHandle, BILLCFG *pBillcfg, char *sIResname);

/* 函数功能:根据数据字典、凭单格式配置、凭单名称生成一张凭单打印数据
 * 输入参数: 
 *   lXmlhandle     内部数据字典XML;
 *   billcfg        格式配置结构;
 *   lXMLHandleData 结果数据XML;
 *   sBillName      凭单名称;
 *   iBillIndex     凭单数据明细索引(张数序号)
 * 返    回: 
 *   输出参数：0--成功、-1--失败
 */
int fpub_GetBillDate(long lXmlHandle, BILLCFG billcfg, long lXMLHandleData,
                     char *sBillName, int iBillIndex);
