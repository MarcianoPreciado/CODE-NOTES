/* 第一章：流程调度SFS
 */

#include "gaps_sfs.h"

/* 以下函数返回值均包括：
 *         成功
 *             MID_SYS_SUCC
 *         失败
 *             MID_SYS_INVALID
 *             MID_SYS_FAIL
 */

/* 功能：调用子系统流程
 * 输入参数: 
 *          HXMLTREE lXmlHandle      RXDA句柄
 *          char* sSubsysName        子系统标识
 * 返    回: 
 *          成功
 *              MID_SYS_SUCC
 *          失败
 *              MID_SYS_INVALID
 *              MID_SYS_FAIL
 *              MID_SYS_SFSINITSYSPARA
 *              MID_SYS_FLOWCALLSYSFLOW
 */
IRESULT fpub_CallSubSysFlow(HXMLTREE lXmlHandle,char* sISubsysName);

/* 功能：调用应用函数
 * 参数：sAppResname       应用资源名称
 * 失败：MID_SYS_FLOWCALLAPP
 */
IRESULT fpub_CallApp(HXMLTREE lXmlHandle,char* sAppResname);

/* 
 * ===  FUNCTION  ==================================================
 *         Name:  fpub_CallAppTrans
 *  Description:  调用应用交易函数
    输入参数: 
                HXMLTREE lXmlHandle      RXDA句柄
                char* sAppResname        应用资源名称
                char* sTransname         内部交易名称
 *  失败：MID_SYS_FLOWCALLAPP
 * =================================================================
 */
IRESULT fpub_CallAppTrans(HXMLTREE lXmlHandle,
                          char* sAppResname,char* sTransname);

/*   失败：MID_SYS_FLOWCALLSUB */
IRESULT fpub_CallFlow(HXMLTREE lXmlHandle, char *sFlowname);

/* 执行交易函数
 * 交易名从当前的内部交易代码结点[XMLNM_ITRANSCODE]获取。
 * 根据该交易名对应的交易执行流程，运行该流程 */
IRESULT fput_ExecTrans(HXMLTREE lXmlHandle);

/* 流程执行总控主函数
 * 失败还返回MID_SYS_FLOWMAINFAIL */
IRESULT fpub_ExecFlow(HXMLTREE lXmlHandle);

/* 初始化应用参数
 * sIAppName为应用标识 */
IRESULT fpub_InitAppPara(HXMLTREE lXmlHandle, char *sIAppName);

/* 初始化子系统参数
 * sSubsysName 为子系统标识 */
IRESULT fpub_InitSubsysPara(HXMLTREE lXmlHandle, char *sSubsysName);

/* 初始化内部交易参数
 * 根据内部交易参数配置表中内部交易的参数配置，初始化内部交易参数 
 * sITransName 为内部交易标识 */
IRESULT fpub_InitTransPara(HXMLTREE lXmlHandle, char *sITransName);

/* 重入流程总控函数
 * 失败还可以返回 MID_SYS_FLOWMAINFAIL */
IRESULT fpub_ReExecFlow(HXMLTREE lXmlHandle);

/* 设置SES回传结构
 * cSucflag为回传交易处理成功标志 */
void fpub_SetSESdhead(SES_DHEAD *pSdhead, char csucflag);


