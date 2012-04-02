#include "gaps_head.h"
EXEC SQL INCLUDE "gaps_head.eh";
EXEC SQL include "ywty_drls.eh"; 
#include "gaps_head.h"

/*生成移动所需的渠道时间 */
IRESULT SYW_XjydTime(HXMLTREE lXmlhandle)
{
  char spara[256],sBuf[1024],sDate[30],sTime[30];
  int iParas;
  
  fpub_InitSoComp(lXmlhandle);
  COMP_PARACOUNTCHK(1);
  COMP_GETPARAS(1,spara,"返回时间节点路径");
  
  strcpy(sDate,GetSysDate());
  strcpy(sTime,GetSysTime());
  
  sprintf(sBuf,"%4.4s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s",sDate,sDate+4,sDate+6,sTime,sTime+2,sTime+4);
  
  COMP_SOFTSETXML(spara,sBuf)
#ifdef DEBUG
  LOG(LM_DEBUG,Fmtmsg("组件[%s]执行结束",fpub_GetCompname(lXmlhandle)),"")  
#endif  
  fpub_SetCompStatus(lXmlhandle,COMPRET_SUCC);
  return COMPRET_SUCC;  	
	
}

/************************************************************************
动态组件函数定义
组件函数名称: XJYD_GenDetailFile
组件名称: 生成新疆移动对明细帐文件
组件参数: 
  序号 参数名称       资源类别        缺省值           是否可空    参数说明
  1    全文件名   XML解析字符串      [/pub/wjm] 否        生成文件文件名
  2    对帐日期   XML解析字符串      [/pub/dzrq] 否        对帐日期
  
  
组件结果状态: 
  结果状态码         结果说明
  缺省

项 目 组: 中间业务发展产品研发组
程 序 员: 
发布日期: 
修改日期: 
************************************************************************/


#define __XARQLSISCUR 1
#define __XARQLSISHIS 0
#define __CSDRLSISCUR 1
#define __CSDRLSISHIS 0

IRESULT XJYD_GenDetailFile(HXMLTREE lXmlhandle)
{
EXEC SQL BEGIN DECLARE SECTION;
  SDB_YWTY_DRLS Sywty_drls;
  int iZbs;
  double dZje;
  char sDzrq[20];
 
  int iCount;
  double idZje=0.0;
  int iCgzbs=0;
  int iCzzbs=0; /* 成功总笔数，返销总笔数 */
  double dCgzje=0.00;
  double dCzzje=0.00; /* 成功总金额，返销总金额 */
  char sXmdh[80];
EXEC SQL END DECLARE SECTION;
  int iParas;
  char sBuf[1024];
  /** 王志斌 20070422 **/
  int isXarqlsdq=__XARQLSISCUR ;/*新澳燃气流水是否为当前还是历史 */
  int iFlag=0;
  
  char sYhbm[60],sSldzbm[60],sSlsxbm[60],sSlbmbm[60],sSlygbm[60];  /*银行编码 受理地州编码 受理市县编码 受理部门编码 受理员工编码*/
  char sFwbsh[80]; /*服务（用户标识号） */
  /*文件指针*/
  FILE *pmxFile;

  /*文件名称, 文件中的一行*/
  char smxFileName[256], sLine[2048];

  fpub_InitSoComp(lXmlhandle);

#ifdef DEBUG
  LOG(LM_DEBUG,Fmtmsg("组件[%s]执行开始",fpub_GetCompname(lXmlhandle)),"")  
#endif 

  /*获取组件参数  --S
  COMP_PARACOUNTCHK(3)
  */
  /*全文件名*/
  memset(smxFileName, 0, sizeof(smxFileName));

  COMP_GETPARSEPARAS(1, sXmdh, "项目代号")  
  COMP_GETPARSEPARAS(2, smxFileName, "新疆移动对帐明细文件名")
  COMP_GETPARSEPARAS(3, sDzrq, "对帐日期")
  COMP_GETPARSEPARAS(4, sYhbm, "银行编码")
  COMP_GETPARSEPARAS(5, sSldzbm, "受理地州编码")
  COMP_GETPARSEPARAS(6, sSlsxbm, "受理市县编码")
  COMP_GETPARSEPARAS(7, sSlbmbm, "受理部门编码")
  COMP_GETPARSEPARAS(8, sSlygbm, "受理员工编码")
  iCount = 0;

  /** 王志斌 20070422 **/
  iCount = 0;
  EXEC SQL SELECT count(*) INTO :iCount
             from ywty_drls
             where xmdh =:sXmdh and zwrq=:sDzrq
               and dzbz='1' and cgbz='0' /*and jyzt='0'*/;
  if(sqlca.sqlcode != 0 || iCount <= 0)
  isXarqlsdq=__XARQLSISHIS;
  else
  isXarqlsdq=__XARQLSISCUR;
  
  LOG(LM_DEBUG,Fmtmsg("查询[%s][%s]%d",sXmdh,sDzrq,isXarqlsdq),"")
  
  /*新疆移动对帐明细文件名*/
  if ((pmxFile = fopen(smxFileName, "wt")) == NULL)
  {
    fpub_SetMsg(lXmlhandle,MID_SYS_WRITEFILE,
                Fmtmsg(MSG_SYS_WRITEFILE, smxFileName, errno, strerror(errno)));
    fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL);
    LOGRET(COMPRET_FAIL,LM_STD,
           Fmtmsg(MSG_SYS_WRITEFILE, smxFileName, errno, strerror(errno)),
           fpub_GetCompname(lXmlhandle))
  }
  
  if(isXarqlsdq==__XARQLSISCUR )
  {
  	EXEC SQL SELECT count(*),sum(jyje) INTO :iCgzbs, :dCgzje
             from ywty_drls
             where xmdh =:sXmdh and zwrq=:sDzrq
               and dzbz='1' and cgbz='0' and jyzt='0';
    if(sqlca.sqlcode != 0 || iCgzbs <= 0)
    dCgzje = 0.00;          
  	EXEC SQL SELECT count(*),sum(jyje) INTO :iCzzbs, :dCzzje
             from ywty_drls
             where xmdh =:sXmdh and zwrq=:sDzrq
               and dzbz='1' and cgbz='0' and jyzt='1';
    if(sqlca.sqlcode != 0 || iCzzbs <= 0)
    dCzzje = 0.00;                              
  }
  else
  {
  	EXEC SQL SELECT count(*),sum(jyje) INTO :iCgzbs, :dCgzje
             from ywty_lsls
             where xmdh =:sXmdh and zwrq=:sDzrq
               and dzbz='1' and cgbz='0' and jyzt='0';
    if(sqlca.sqlcode != 0 || iCgzbs <= 0)
    dCgzje = 0.00;               
  	EXEC SQL SELECT count(*),sum(jyje) INTO :iCzzbs, :dCzzje
             from ywty_lsls
             where xmdh =:sXmdh and zwrq=:sDzrq
               and dzbz='1' and cgbz='0' and jyzt='1';
    if(sqlca.sqlcode != 0 || iCzzbs <= 0)
    dCzzje = 0.00;                              
  }
  /* 20090225 保证金额不带小数点 sprintf(sLine, "%d\t%.2f\t%d\t%.2f\t%d\t%.2f\n", iCgzbs,dCgzje*100,iCzzbs,dCzzje*100,iCgzbs+iCzzbs,(dCgzje+dCzzje)*100); */
  sprintf(sLine, "%d\t%.0f\t%d\t%.0f\t%d\t%.0f\n", iCgzbs,dCgzje*100,iCzzbs,dCzzje*100,iCgzbs+iCzzbs,(dCgzje+dCzzje)*100); /* 20090225 保证金额不带小数点 */
  fputs(sLine, pmxFile);       /*加入明细信息*/
            
  /*燃气用户号、费用月份(hcq字段)、交易金额、交易日期、交易类别(G)、机构号、流水号 */
  if(isXarqlsdq==__XARQLSISCUR )
  {
  iFlag=1;
  EXEC SQL DECLARE cur_xarqdetail1 CURSOR FOR 
           select hcq,zhqzlsh,fkpzh,yhh,jyje,jyzt
           from ywty_drls 
             where xmdh =:sXmdh and zwrq=:sDzrq
               and dzbz='1' and cgbz='0' /*and jyzt='0'*/
           order by zhqzlsh;
  }
  else
  if(isXarqlsdq==__XARQLSISHIS )
  {
  iFlag=3;
  EXEC SQL DECLARE cur_xarqdetail3 CURSOR FOR 
           select hcq,zhqzlsh,fkpzh,yhh,jyje,jyzt
           from ywty_lsls 
             where xmdh =:sXmdh and zwrq=:sDzrq
               and dzbz='1' and cgbz='0' /*and jyzt='0'*/
           order by zhqzlsh;
  }
  if(iFlag == 1)
  EXEC SQL OPEN cur_xarqdetail1;
  else
  if(iFlag == 3)
  EXEC SQL OPEN cur_xarqdetail3;
  SQLERRLOG(LM_STD,Fmtmsg(MSG_SQL_CUROPEN,"cur_xarqdetail",
            sqlca.sqlcode),fpub_GetCompname(lXmlhandle))
  if ( SQLERR )
  {
    /*关闭文件*/
    fclose(pmxFile);
    fpub_SetMsg(lXmlhandle,MID_SQL_CUROPEN,
         Fmtmsg(MSG_SQL_CUROPEN,"cur_xarqdetail",sqlca.sqlcode));
    fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL); 
    LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg(MSG_SQL_CUROPEN,
           "cur_xarqdetail",sqlca.sqlcode),fpub_GetCompname(lXmlhandle))
  }
  memset(&Sywty_drls, 0, sizeof(Sywty_drls));
  if(iFlag == 1)
  EXEC SQL FETCH cur_xarqdetail1 into 
              :Sywty_drls.hcq,:Sywty_drls.zhqzlsh,:Sywty_drls.fkpzh,:Sywty_drls.yhh,:Sywty_drls.jyje,:Sywty_drls.jyzt;
  else
  if(iFlag == 3)
  EXEC SQL FETCH cur_xarqdetail3 into 
           :Sywty_drls.hcq,:Sywty_drls.zhqzlsh,:Sywty_drls.fkpzh,:Sywty_drls.yhh,:Sywty_drls.jyje,:Sywty_drls.jyzt;

  SQLERRLOG(LM_STD,Fmtmsg(MSG_SQL_CURFETCH,"cur_xarqdetail",
            sqlca.sqlcode),fpub_GetCompname(lXmlhandle))
  if ( SQLERR )
  {
    fclose(pmxFile);
    if(iFlag == 1)
    {
    EXEC SQL CLOSE cur_xarqdetail1;
    EXEC SQL FREE cur_xarqdetail1;
    }
    else
    {
    EXEC SQL CLOSE cur_xarqdetail3;
    EXEC SQL FREE cur_xarqdetail3;
    }
    fpub_SetMsg(lXmlhandle,MID_SQL_CURFETCH,
         Fmtmsg(MSG_SQL_CURFETCH,"cur_xarqdetail",sqlca.sqlcode));
    fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL); 
    LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg(MSG_SQL_CURFETCH,
         "cur_xarqdetail",sqlca.sqlcode),fpub_GetCompname(lXmlhandle))
  } 
  while (SQLSUCC)
  {
    char sDqdm[20],sFjbz[20],sYhjfsj[80],sYdjfsj[80];  /*用户归属地州编码;fjbz;银行缴费时间;移动缴费时间;*/
    memset(sLine, 0, sizeof(sLine));
    trim(Sywty_drls.hcq);
    trim(Sywty_drls.fkpzh);
    trim(Sywty_drls.yhh);

    memset(sDqdm,0x00,sizeof(sDqdm));
    GetListbyDiv(Sywty_drls.hcq,1,sDqdm,';','\\');
    memset(sFjbz,0x00,sizeof(sFjbz));
    GetListbyDiv(Sywty_drls.hcq,2,sFjbz,';','\\');
    memset(sYhjfsj,0x00,sizeof(sYhjfsj));
    GetListbyDiv(Sywty_drls.hcq,3,sYhjfsj,';','\\');
    if(strstr(sYhjfsj,"-") && strstr(sYhjfsj,":"))
    {
    	StrReplace(sYhjfsj,"-","");
    	StrReplace(sYhjfsj,":","");
    	StrReplace(sYhjfsj," ","");
    }
    memset(sYdjfsj,0x00,sizeof(sYdjfsj));
    GetListbyDiv(Sywty_drls.hcq,4,sYdjfsj,';','\\');
    if(strstr(sYdjfsj,"-") && strstr(sYdjfsj,":"))
    {
    	StrReplace(sYdjfsj,"-","");
    	StrReplace(sYdjfsj,":","");
    	StrReplace(sYdjfsj," ","");
    }    
    memset(sFwbsh,0x00,sizeof(sFwbsh));
    GetListbyDiv(Sywty_drls.hcq,5,sFwbsh,';','\\');    
    
    /* 20080131 修改 */
    if(Sywty_drls.jyzt[0] == '0') /* 正常交易 */
    sprintf(sLine, "%s\t%s\t%s\t%s\t%s\t%s\t%8.8s%08d\t%s\t%s\t%s\t%s\t%.0f\t%s\t%s\t%s\t%s\t%s\t%s\n",
    							 sYhbm,sSldzbm,sSlsxbm,sSlbmbm,sSlygbm,  /*银行编码 受理地州编码 受理市县编码 受理部门编码 受理员工编码*/
                   sDqdm,sDzrq,Sywty_drls.zhqzlsh,sYhjfsj,Sywty_drls.fkpzh,sYdjfsj, /*用户归属地州编码 银行收费流水 银行收费时间（YYYYMMDDHH24MISS） 移动收费流水 收费时间（YYYYMMDDHH24MISS）*/
                   sFwbsh,Sywty_drls.jyje*100,"0","0","0",/*用户标志号 实缴金额 交费方式 缴费卡类型 缴费卡号码*/
                   sFjbz,"0","0"); /*是否复机（销号用户）返销标志(‘０’-未返销,‘1’-返销) 银行返销时间（YYYYMMDDHH24MISS）*/
     else /* 返销交易 */
    sprintf(sLine, "%s\t%s\t%s\t%s\t%s\t%s\t%8.8s%08d\t%s\t%s\t%s\t%s\t%.f\t%s\t%s\t%s\t%s\t%s\t%8.8s%6.6s\n",
    							 sYhbm,sSldzbm,sSlsxbm,sSlbmbm,sSlygbm,  /*银行编码 受理地州编码 受理市县编码 受理部门编码 受理员工编码*/
                   sDqdm,sDzrq,Sywty_drls.zhqzlsh,sYhjfsj,Sywty_drls.fkpzh,sYdjfsj, /*用户归属地州编码 银行收费流水 银行收费时间（YYYYMMDDHH24MISS） 移动收费流水 收费时间（YYYYMMDDHH24MISS）*/
                   sFwbsh,Sywty_drls.jyje*100,"0","0","0",/*用户标志号 实缴金额 交费方式 缴费卡类型 缴费卡号码*/
                   sFjbz,"1",sDzrq,sYdjfsj); /*是否复机（销号用户）返销标志(‘０’-未返销,‘1’-返销) 银行返销时间（YYYYMMDDHH24MISS）*/

    fputs(sLine, pmxFile);       /*加入明细信息*/ 
    memset(&Sywty_drls, 0, sizeof(Sywty_drls));
    if(iFlag == 1)
    EXEC SQL FETCH cur_xarqdetail1 into 
           :Sywty_drls.hcq,:Sywty_drls.zhqzlsh,:Sywty_drls.fkpzh,:Sywty_drls.yhh,:Sywty_drls.jyje,:Sywty_drls.jyzt;
    else
    if(iFlag == 3)
    EXEC SQL FETCH cur_xarqdetail3 into 
           :Sywty_drls.hcq,:Sywty_drls.zhqzlsh,:Sywty_drls.fkpzh,:Sywty_drls.yhh,:Sywty_drls.jyje,:Sywty_drls.jyzt;
    SQLERRLOG(LM_STD,Fmtmsg(MSG_SQL_CURFETCH,"cur_xarqdetail",
           sqlca.sqlcode),fpub_GetCompname(lXmlhandle))
    if ( SQLERR )
    {
      /*关闭文件*/
      fclose(pmxFile);
    if(iFlag == 1)
    {

    EXEC SQL CLOSE cur_xarqdetail1;
    EXEC SQL FREE cur_xarqdetail1;
    }
    else
    {
    EXEC SQL CLOSE cur_xarqdetail3;
    EXEC SQL FREE cur_xarqdetail3;
    }
      
      fpub_SetMsg(lXmlhandle,MID_SQL_CURFETCH,
           Fmtmsg(MSG_SQL_CURFETCH,"cur_xarqdetail",sqlca.sqlcode));
      fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL); 
      LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg(MSG_SQL_CURFETCH,
           "cur_xarqdetail",sqlca.sqlcode),fpub_GetCompname(lXmlhandle))
    }  
    
  }/*end of SQLNOTFOUND*/
  
  /*关闭文件*/
  fclose(pmxFile);
   
    if(iFlag == 1)
    {

    EXEC SQL CLOSE cur_xarqdetail1;
    EXEC SQL FREE cur_xarqdetail1;
    }
    else
    {
    EXEC SQL CLOSE cur_xarqdetail3;
    EXEC SQL FREE cur_xarqdetail3;
    }

  fpub_SetCompStatus(lXmlhandle,COMPSTATUS_SUCC);
  
#ifdef DEBUG
  LOG(LM_DEBUG,Fmtmsg("组件[%s]执行结束",fpub_GetCompname(lXmlhandle)),"")  
#endif
  
  return COMPRET_SUCC;
}

/* 新疆移动，将输入的XML节点对应的金额除以/100 */
IRESULT XJYD_MoneyDiv(HXMLTREE lXmlhandle)
{
  char sdata[200],sbuf[200],spara[200],sFmt[254];
  int iParas,i,ilen,j;
  int iBcs,iBlws; /*被除数、小数点后保留位数*/
  fpub_InitSoComp(lXmlhandle);
  COMP_GETPARSEPARAS(1,sbuf,"被除数（比如100）")
  iBcs = atoi(sbuf);
  COMP_GETPARSEPARAS(2,sbuf,"小数点后保留位数")
  iBlws = atoi(sbuf);
  
  sprintf(sFmt,"%%.%df",iBlws);
  
  if ( (iParas=fpub_GetPARAcount(lXmlhandle)) < 0 )
  {
    fpub_SetMsg(lXmlhandle,MID_SYS_COMPEXEC,
                Fmtmsg(MSG_SYS_COMPEXEC,fpub_GetCompname(lXmlhandle)));
    fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL);
    LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg(MSG_SYS_COMPCOUNTPARA,
           fpub_GetCompname(lXmlhandle)),"SDATA_Strcat")
  }
  if (iParas < 3 )
  {
    fpub_SetMsg(lXmlhandle,MID_SYS_COMPEXEC,
                Fmtmsg(MSG_SYS_COMPEXEC,fpub_GetCompname(lXmlhandle)));
    fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL);
    LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg("参数个数错<3,paras[%d]",
           iParas),"SDATA_Strcat")
  }	
  memset(sbuf,0,sizeof(sbuf));
  /*COMP_GETPARA(1,sdata); */
  for (i=3;i<=iParas;i++)
  {
    COMP_GETPARA(i,spara)
    memset(sbuf,0,sizeof(sbuf));
    memset(sdata,0,sizeof(sdata));
    COMP_SOFTGETXML(spara,sbuf)
    trim(sbuf);
    if(strlen(sbuf) > 0)
    {
    	sprintf(sdata,sFmt,atof(sbuf)/iBcs);
    	COMP_SOFTSETXML(spara,sdata)
    }
 
    LOG(LM_STD,Fmtmsg("处理[%s]的节点对应的金额值[%s]为[%s]",spara,sbuf,sdata),"");
  }
  fpub_SetCompStatus(lXmlhandle,COMPRET_SUCC);
  return COMPRET_SUCC;  	
}

/**
	新疆移动批量代扣处理   
	实现从签约明细表 ywyy_cib_dlwtgx 读出当月未扣的签约记录，发起自动代扣交易处理。    
**/
IRESULT XJYD_PLDK(HXMLTREE lXmlhandle)
{
EXEC SQL BEGIN DECLARE SECTION;
  char sJyzh[20],sKhbs[30],sZwrq[60],sPzdh[20],sQymm[10],sQkfs[10],sZhzl[10];
  char sFyrq[30],sScjysj[30]; /* 费用日期,上次交易时间 */ 
  int iCount;
  char sXmdh[80];
  double dXyje;
EXEC SQL END DECLARE SECTION;
  int iParas;
  char sBuf[2048];
  char sFlow[100],sProMode[60];
  int iFlag;
  int isqlcode,ik,ij;
  char sJyzhnode[80],sKhbsnode[80],sBslxnode[80],sPzdhnode[80];
  
  fpub_InitSoComp(lXmlhandle);

#ifdef DEBUG
  LOG(LM_DEBUG,Fmtmsg("组件[%s]执行开始",fpub_GetCompname(lXmlhandle)),"")  
#endif 

  /*获取组件参数  --S
  COMP_PARACOUNTCHK(4) */
  
  COMP_GETPARSEPARAS(1, sXmdh, "项目代号")
  COMP_GETPARSEPARAS(2, sZwrq, "账务日期")
  trim(sZwrq);   
  COMP_GETPARSEPARAS(3,sFlow,"发送代扣交易处理流程名")
  COMP_GETPARSEPARAS(4,sProMode,"流程处理模式") /*流程模式,流程执行的结果是否影响文件转换过程 0 不影响, 1 若流程执行失败 失败结束 2 若流程执行失败 文件转换忽略该失败,跳过本行,继续进行 */
  COMP_GETPARAS(5, sJyzhnode, "返回交易账号节点")
  COMP_GETPARAS(6, sKhbsnode, "返回客户标识节点")
  COMP_GETPARAS(7, sBslxnode, "返回标识类型节点")
  COMP_GETPARAS(8, sPzdhnode, "返回凭证代号节点")
  /**
  COMP_GETPARAS(7, pzhqzlsh, "返回流水号")
  COMP_GETPARAS(8, pisend, "返回最后一笔标识")
     
  COMP_HARDGETXML("/pub/zwrq",sZwrq)
  trim(sZwrq);
  */
  memset(sFyrq,0x00,sizeof(sFyrq));
  memcpy(sFyrq,sZwrq,6);
  
  EXEC SQL DECLARE cur_ywty_drls CURSOR for
         select jyzh,pzdh, khbs,qymm,qkfs,xyje,zhzl from ywyy_cib_dlwtgx 
         where ywlx =:sXmdh and fyrq<>:sFyrq
               and zt='0';

  EXEC SQL OPEN cur_ywty_drls;
  if ( sqlca.sqlcode != 0 )
  {
	       isqlcode = sqlca.sqlcode;
         fpub_SetMsg(lXmlhandle,MID_SYS_COMPEXEC,
                Fmtmsg("open cursor 失败[%d]",isqlcode));
         fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL);
         LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg("open cursor 失败[%d]",isqlcode), 
				fpub_GetCompname(lXmlhandle))
   }

   memset(sJyzh,0x00,sizeof(sJyzh));
   memset(sKhbs,0x00,sizeof(sKhbs));
   memset(sPzdh,0x00,sizeof(sPzdh));
   memset(sQymm,0x00,sizeof(sQymm));
   memset(sQkfs,0x00,sizeof(sQkfs));
   memset(sZhzl,0x00,sizeof(sZhzl));
   dXyje = 0.00;  
   EXEC SQL FETCH cur_ywty_drls into :sJyzh,:sPzdh,:sKhbs,:sQymm,:sQkfs,:dXyje,:sZhzl;
     if ( sqlca.sqlcode != 0 )
     {
         isqlcode = sqlca.sqlcode;
         	EXEC SQL CLOSE cur_ywty_drls;
	 				EXEC SQL FREE cur_ywty_drls; /* 20070828*/
 			 	 			 
         fpub_SetMsg(lXmlhandle,MID_SYS_COMPEXEC,
                Fmtmsg("fetch cursor 失败[%d]",isqlcode));
         fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL);
         LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg("fetch cursor 失败[%d]",isqlcode), 
					fpub_GetCompname(lXmlhandle))
     }
     
     while(1)
     {
   		trim(sJyzh);     
      COMP_SOFTSETXML(sJyzhnode,sJyzh)
   		trim(sPzdh);     
      COMP_SOFTSETXML(sPzdhnode,sPzdh)
      
      sQymm[6] = 0x00;
      COMP_SOFTSETXML("/pub/jymm",sQymm)
      sQkfs[1] = 0x00;
      COMP_SOFTSETXML("/pub/qkfs",sQkfs)
          
   		trim(sKhbs);
   		
   		sprintf(sBuf,"%.2f",dXyje);
   		COMP_SOFTSETXML("/pub/xyje",sBuf)
   		
   		sZhzl[1] = 0x00;
      COMP_SOFTSETXML("/pub/zhzl",sZhzl)
      
   		/* 20090302 新增处理 */
   		EXEC SQL update ywyy_cib_dlwtgx set clbs='9',xyxx='开始处理' where ywlx=:sXmdh and jyzh=:sJyzh and khbs=:sKhbs;
   		
   		/**
   		if(strlen(sKhbs) == 8 || strlen(sKhbs) == 7)
   		sprintf(sKhbs,"0991%s",sKhbs);  如果标识号没有带区号则自动加上区号*/     
      COMP_SOFTSETXML(sKhbsnode,sKhbs)
      
     
      /*调用子流程进行代扣处理*/
         if ( fpub_CallFlow(lXmlhandle,sFlow) != MID_SYS_SUCC)
         {

         		EXEC SQL CLOSE cur_ywty_drls;
	 					EXEC SQL FREE cur_ywty_drls; /* 20070828*/
         fpub_SetMsg(lXmlhandle,MID_SYS_COMPEXEC,
                Fmtmsg("调用执行流程[%s] 失败",sFlow));
         fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL);
         LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg("调用执行流程[%s] 失败",sFlow), 
					fpub_GetCompname(lXmlhandle))
         }

         /*判断子流程是否处理成功*/
         memset(sBuf,0,sizeof(sBuf));
        COMP_SOFTGETXML("/pub/respcode",sBuf)
        ik = 1;
        ij = 0;
        while(sBuf[ij] != 0x00)
        {
					if(sBuf[ij] != '0')
					{
						ik = 0;
						break;
					}
					ij++;
        }
        if ( !ik )
        {
	 				if(sProMode[0] == '1')
         {

         		EXEC SQL CLOSE cur_ywty_drls;
	 					EXEC SQL FREE cur_ywty_drls; /* 20070828*/
         fpub_SetMsg(lXmlhandle,MID_SYS_COMPEXEC,
                Fmtmsg("调用执行流程[%s] 失败",sFlow));
         fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL);
         LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg("调用执行流程[%s] 失败",sFlow), 
				fpub_GetCompname(lXmlhandle))
				}
				else
				COMP_SOFTSETXML("/pub/respcode","00000")  /* 20080509 既然是忽略了,就强制设置内部响应码为成功 */
	
       }
       
   		memset(sJyzh,0x00,sizeof(sJyzh));
   		memset(sKhbs,0x00,sizeof(sKhbs));
   		memset(sPzdh,0x00,sizeof(sPzdh));
   		memset(sQymm,0x00,sizeof(sQymm));
   		memset(sQkfs,0x00,sizeof(sQkfs));
   		memset(sZhzl,0x00,sizeof(sZhzl));   
   		EXEC SQL FETCH cur_ywty_drls into :sJyzh,:sPzdh,:sKhbs,:sQymm,:sQkfs,:dXyje,:sZhzl;
   		if(sqlca.sqlcode != 0)
   		break;
    }

         	EXEC SQL CLOSE cur_ywty_drls;
	 				EXEC SQL FREE cur_ywty_drls; /* 20070828*/
#ifdef DEBUG
    LOG(LM_DEBUG,Fmtmsg("组件[%s]执行结束",fpub_GetCompname(lXmlhandle)),"")
#endif
       fpub_SetCompStatus(lXmlhandle,COMPSTATUS_SUCC);
       return COMPRET_SUCC;
}