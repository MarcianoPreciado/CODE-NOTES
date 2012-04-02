/* 第三章：组件开发辅助函数组
 */

/* 所有函数除特殊说明，返回值均为：
 *
    成功
        MID_SYS_SUCC
    失败
        MID_SYS_INVALID
        MID_SYS_FAIL
 */

IRESULT fpub_InitSoComp(HXMLTREE lXmlHandle);
IRESULT fpub_SetCompStatus(HXMLTREE lXmlHandle, long lStatus);

/* 输入参数：
 * iParaid      参数序号
 * pParabuf     组件参数内容缓冲区指针
 * iSize        缓冲区大小 */
IRESULT fpub_GetPARA(HXMLTREE lXmlHandle, int iParaid, char *pParabuf, int iSize);
IRESULT fpub_GetParsePARA(HXMLTREE lXmlHandle, int iParaid, char *pParabuf, int iSize);

int fpub_GetPARAcount(HXMLTREE lXmlHandle);     /* 如果成功，返回大于等于零的整形值。如负数，则失败。 */

/* 设置内部响应码和响应信息 */
IRESULT fpub_SetMsg(HXMLTREE lXmlHandle, int iMsgid, char *sMsg);

/* 设置指定组件参数 */
IRESULT fpub_SetCompPara(HXMLTREE lXmlHandle, int iParasn, char *pParabuf);

