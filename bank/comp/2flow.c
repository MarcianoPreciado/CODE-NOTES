/* 第二章：平台流程组件操作函数组 
 */

/* 以下函数成功时均返回 MID_SYS_SUCC
 * 失败时均包括 MID_SYS_FAIL */
/* 运行动态函数组件
 * RMA_COMP SComp 组件登记结构 */
IRESULT fpub_CallDfuncComp(HXMLTREE lXmlHandle, RMA_COMP SComp);

/* 释放最后执行的动态组件库
 *
 * 该函数使用全局变量gpComplibhandle
 * 在动态组件库非预载入模式时，若流程运行该过程产生异常终止导致未正常释放时，
 * 用于释放最后被打开的动态函数库。*/
void fpub_FreeLastDfunc(void);

/* 运行程序组件 */
IRESULT fpub_CallProgComp(HXMLTREE lXmlHandle, RMA_COMP SComp);

/* 运行静态函数组件 */
IRESULT fpub_CallSfuncComp(HXMLTREE lXmlHandle, RMA_COMP SComp);

RMA_FLOWSTAT *rma_GetFlowCompStat(RMA_FLOW *pSflowComp, long iCompStatus);

/* 获取流程首组件结构地址
 * sIFlowname               流程名称
 * RMA_FLOW *pFirstComp    流程首组件结构地址（输出）*/
IRESULT fpub_GetFirstFlowcomp(HXMLTREE lXmlHandle,
                              char *sIFlowname, RMA_FLOW *pFirstComp);

/* 函数功能:获取流程后续组件结构
 * 输入参数: 
 *          HXMLTREE lXmlHandle      RXDA句柄
 *          char* sFlowname          流程名称
 *          long lIFlowsn            当前流程组件序号
 *          long lICurStat           当前组件执行结果状态
 *          RMA_FLOW* pFlowcomp      流程后续组件结构指针
 */
IRESULT fpub_GetNextFlowcomp(HXMLTREE lXmlHandle,char* sIFlowname,
                             long lIFlowsn,long lICurStat,RMA_FLOW* pFlowcomp);

/* 初始化组件参数
 * 把组件参数从RMA数据区中提放到RXDA中
 * 失败还返回值MID_SYS_INVALID */
IRESULT fpub_InitFlowcompPara(HXMLTREE lXmlHandle, char *sIFlowname, long lIFlowsn);

/* 获取组件自愿注册信息
 * RMA_COMP *pComp 组件结构指针
 * 失败还返回值MID_SYS_INVALID */
IRESULT fpub_GetCompres(HXMLTREE lXmlHandle, char *sICompname, RMA_COMP *pComp);

/* 根据交易名获取流程名
 * 从内部交易和执行流程的对应关系中获取流程名 
 * 失败还返回值MID_SYS_INVALID */
IRESULT fpub_GetFlowByTrans(HXMLTREE lXmlHandle, char *sITransname, char *sFlowname);

