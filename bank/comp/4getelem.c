/* 第四章：GAPS固定结点获取操作函数组
 */
char *fpub_GetSubsysname(HXMLTREE lXmlHandle);  /* 从XMLNM_SESSVR结点获取当前子系统名称 */

char *fpub_GetAppname(HXMLTREE lXmlHandle);     /* 从XMLNM_APPCODE节点获取当前的应用名称 */

char *fpub_GetLastFlowname(HXMLTREE lXmlHandle); /* 从XMLNM_FLOWNAME节点获取当前的流程名称 */

char *fpub_GetSysTraceNo(HXMLTREE lXmlHandle);  /* 获取当前系统跟踪号 */

char *fpub_GetCompname(HXMLTREE lXmlHandle);

char *fpub_GetLastTrans(HXMLTREE lXmlHandle);   /* 从XMLNM_ITRANSCODE节点获取当前的内部交易名 */

long fpub_GetRmahead(HXMLTREE lXmlHandle);      /* 从XMLNM_RMAHEAD节点获取当前的内部交易名 */
