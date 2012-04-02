/* 第六章：平台RMA操作函数组
 */
#include "gaps_rma.h"

/*-----------------------------------------------------------------------------
 *  初始化资源函数
 *-----------------------------------------------------------------------------*/
/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  rma_InitRma
 *  Description:  初始化RMA资源共享区
 *  输入参数：
 *          long lStart     RMA初始地址
 *          long *lMaxlen   RMA区最大长度（返回实际使用长度）
 *          int  iMargin    RMA资源空间调整余量比例
 *  返回值：
 *          成功    MID_SYS_SUCC
 *          失败    MID_SYS_RMAINIT
 *                  MID_SYS_RMAINITFLOW
 *                  MID_SYS_RMAINITCOMP
 *                  MID_SYS_RMANOHEADSPACE
 * =========================================================================
 */
IRESULT rma_InitRma(long lStart, long *lMaxlen, int iMargin);

/* 
 * ===  FUNCTION  ==========================================================
 *         Name:  rma_InitAppres
 *  Description:  初始化RMA应用资源
 *  输入参数：
 *          long lCurOffset     当前资源起始偏移量
 *          RMA_RESCLASS *pResClass 资源类型索引结构
 *          long *lResSize      资源占用空间
 *          int iMargin         RMA资源空间调整余量比例
 *  返回：
 *       成功       MID_SYS_SUCC
 *       失败       MID_SYS_RMANOSPACE
 *                  MID_SYS_RMAINITAPP
 * =========================================================================
 */
IRESULT rma_InitAppres(long lCurOffset, RMA_RESCLASS *pResClass, 
                       long *lResSize, int iMargin);

IRESULT rma_InitSubsysres(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin);

IRESULT rma_InitTransres(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin); /* 初始化内部交易资源 */

IRESULT rma_InitFlowres(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin);

IRESULT rma_InitCompres(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin);

IRESULT rma_InitFuncres(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin);

IRESULT rma_InitTstructres(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin); /* 初始化RMA表结构资源 */

IRESULT rma_InitFileres(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin);

IRESULT rma_InitICXPres(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin);

IRESULT rma_Init8583Pres(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin);

IRESULT rma_InitXMLPres(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin);

IRESULT rma_InitMonres(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin);

IRESULT rma_InitBillpres(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin); /* 凭单结构资源 */

IRESULT rma_InitRescoderes(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin);

IRESULT rma_IniteEnumres(long lCurOffset, RMA_RESCLASS *pResClass,
                          long *lResSize, int iMargin);


/*-----------------------------------------------------------------------------
 *  更新资源函数
 *-----------------------------------------------------------------------------*/
/* 刷新RMA资源数据
 * 该函数获取非当前活动的RMA，将资源配置数据初始化到该区域 */
IRESULT rma_RefreshRma(long lRmaStart, long *lMaxlen, int iMargin, int iRefreshMode);

/* 增加指定资源RMA数据
 * 输入参数：
 *          long lRmaStart      RMA区起始地址
 *          char * sIResname    资源名称
 *          long lIResclass     资源类别
 *          int iRefreshMode    刷新模式
 * 返回：
 *          MID_SYS_RMACOUNTRESSIZE
 *          MID_SYS_MEMALLOC
 *          MID_SYS_RMAREFRESHRES
 *          MID_SYS_RMANOSPACE
 */
IRESULT rma_AddResRma(long lRmaStart, char *sResname, long lResclass, int iRefreshMode);

/* 刷新RMA中指定资源数据
 * 输入参数: 
 *          long lRmaStart       RMA区起始地址
 *          char* sIResname      资源名称
 *          long lIResclass      资源类别
 *          int iRefreshMode     刷新模式
 * 返    回: 
 *          MID_SYS_RMACOUNTRESSIZE
 *          MSG_SYS_RMAGETRESIDX        当前RMA中不存在该资源 
 *          MID_SYS_MEMALLOC            
 *          MSG_SYS_RMAREFRESHRES
 *          MID_SYS_RMANOSPACE          调整余量区域空间不足
 */
IRESULT rma_RefreshResRma(long lRmaStart,char* sResname,
                          long lResclass,int iRefreshMode);

/* 返    回: 
 *          MID_SYS_RMACOUNTRESSIZE
 *          MSG_SYS_RMAGETRESIDX        当前RMA中不存在该资源 
 */
IRESULT rma_DelResRma(long lRmaStart,char* sResname,
                          long lResclass,int iRefreshMode);

/*-----------------------------------------------------------------------------
 *  获取资源函数
 *  成功均返回值MID_SYS_SUCC
 *  失败均返回值MID_SYS_FAIL
 *-----------------------------------------------------------------------------*/

/* 获取指定子系统资源的实体结构地址
 * 输入参数：
 *          long lRmaStart      RMA区起始地址
 *          char *sSubsysname   子系统名称
 *          long *pSSubsys      子系统资源的实体结构地址（输出）
例:
RMA_SUBSYS *pSSubsys;

  if ( (lRmaStart = fpub_GetRmahead(lXmlHandle)) == 0 )
    LOGRET(MID_SYS_GETRMAHEAD,LM_STD,
           MSG_SYS_GETRMAHEAD,"fpub_InitSubsysPara")

  if ( rma_GetSubsysres(lRmaStart, sISubsysName, (long *)&pSSubsys)  != MID_SYS_SUCC )
    LOGRET(MID_SYS_FAIL,LM_STD,
           Fmtmsg(MSG_SYS_RMAGETSUBSYS,sISubsysName),"fpub_InitSubsysPara")
 */
IRESULT rma_GetSubsysres(long lRmaStart, char *sSubsysname, long *pSSubsys);

IRESULT rma_GetAppres(long lRmaStart,char* sAppname, long *pSApp);

IRESULT rma_GetFlowComp(long lRmaStart,char* sFlowName,
                        long lFlowsn,long* pSComp);

IRESULT rma_GetTransres(long lRmaStart,char* sTransname, long *pSTrans);

IRESULT rma_GetCompres(long lRmaStart,char* sCompname, long *pSComp);

IRESULT rma_GetFuncres(long lRmaStart,char* sFuncname, long *pSFunc);

IRESULT rma_GetResclass(long lRmaStart ,long lResclass,long *plResclassStart); /* 指定资源类型 */

IRESULT rma_GetResIndex(long lRmaStart,long lResclass,
                        char* sResname,long* plResIdxStart,long* plResClass);

IRESULT rma_GetResCfg(long lRmaStart,long lResclass,
                      char* sResname,long* plResStart); /* 指定资源的实体数据入口地址 */
