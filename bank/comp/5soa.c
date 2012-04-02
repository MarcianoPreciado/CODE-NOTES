/* 第五章：平台SOA操作函数组 
 */

IRESULT soa_InitSoa(long lGSAhead); /* lGSAhead     GSA首地址 */

IRESULT soa_UpdateSoaSo(long lGSAhead, char *sSolibname); /* 更新SO结构登记数组（SORA) */

IRESULT soa_DelSoaSo(long lGSAhead, char *sSolibname);

/* 在SOA搜索动态库的更新记录，查询时否需要重载该库 
 * 返回0    不需要更新该库
 * 返回1    需要更新*/
IRESULT soa_SoNeedUpdate(GSA_SOA *pSsoa, GAPS_USOREG *pSusoreg);

/* 函数功能:
 * 当服务程序在运行时要运行某个动态链接库时,首先访问 SOUA,察看是否已经打开
 * ,若已经打开,则访问 SORA,察看是否需要更新(比对时间戳),若需要更新,则
 * 重新打开该动态链接库、更新 SOUA 并执行该库;如果尚未打开,则打开该库、更新
 * SOUA 并执行该库。
 *
 * 输入参数:
 * GSA_SOA *pSsoa       SO 结构登记数组(SORA)
 * GSA_SOUA *pSoua      内部 SO 结构使用数组(SOUA)指针
 * char* sSolibname     动态链接库名称
 * char* sDftsodir      缺省动态链接库路径
 * long* pSolibaddr     动态链接库地址
 */
IRESULT soa_GetSouaSo(GSA_SOA *pSsoa, GSA_SOUA *pSoua, char *sSolibname, 
                      char *sDftsodir, long *pSolibaddr);

IRESULT soa_InitSOUA(long lGSAhead, GSA_SOUA *pSoua, GAPS_INI Sgapsini);

IRESULT soa_FreeSOUA(GSA_SOUA *pSoua);

/* 输入参数:
 * char* sSolibname     动态库名称
 * char* sDftsodir      缺省动态库路径
 * long *pSolibaddr     动态库地址返回指针
 * 返回:
 * MID_SYS_SUCC
 * MID_SYS_FLOWOPENSO
 * 
 * long lSoaddr;
 * if ( fpub_dlopen(sSolibname,sDftsodir,&lSoaddr)!=MID_SYS_SUCC )
     ;
 */
IRESULT fpub_dlopen(char *sSolibname, char *sDftsodir, long *pSolibaddr);

