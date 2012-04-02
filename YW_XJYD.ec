#include "gaps_head.h"
EXEC SQL INCLUDE "gaps_head.eh";
EXEC SQL include "ywty_drls.eh"; 
#include "gaps_head.h"

/*�����ƶ����������ʱ�� */
IRESULT SYW_XjydTime(HXMLTREE lXmlhandle)
{
  char spara[256],sBuf[1024],sDate[30],sTime[30];
  int iParas;
  
  fpub_InitSoComp(lXmlhandle);
  COMP_PARACOUNTCHK(1);
  COMP_GETPARAS(1,spara,"����ʱ��ڵ�·��");
  
  strcpy(sDate,GetSysDate());
  strcpy(sTime,GetSysTime());
  
  sprintf(sBuf,"%4.4s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s",sDate,sDate+4,sDate+6,sTime,sTime+2,sTime+4);
  
  COMP_SOFTSETXML(spara,sBuf)
#ifdef DEBUG
  LOG(LM_DEBUG,Fmtmsg("���[%s]ִ�н���",fpub_GetCompname(lXmlhandle)),"")  
#endif  
  fpub_SetCompStatus(lXmlhandle,COMPRET_SUCC);
  return COMPRET_SUCC;  	
	
}

/************************************************************************
��̬�����������
�����������: XJYD_GenDetailFile
�������: �����½��ƶ�����ϸ���ļ�
�������: 
  ��� ��������       ��Դ���        ȱʡֵ           �Ƿ�ɿ�    ����˵��
  1    ȫ�ļ���   XML�����ַ���      [/pub/wjm] ��        �����ļ��ļ���
  2    ��������   XML�����ַ���      [/pub/dzrq] ��        ��������
  
  
������״̬: 
  ���״̬��         ���˵��
  ȱʡ

�� Ŀ ��: �м�ҵ��չ��Ʒ�з���
�� �� Ա: 
��������: 
�޸�����: 
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
  int iCzzbs=0; /* �ɹ��ܱ����������ܱ��� */
  double dCgzje=0.00;
  double dCzzje=0.00; /* �ɹ��ܽ������ܽ�� */
  char sXmdh[80];
EXEC SQL END DECLARE SECTION;
  int iParas;
  char sBuf[1024];
  /** ��־�� 20070422 **/
  int isXarqlsdq=__XARQLSISCUR ;/*�°�ȼ����ˮ�Ƿ�Ϊ��ǰ������ʷ */
  int iFlag=0;
  
  char sYhbm[60],sSldzbm[60],sSlsxbm[60],sSlbmbm[60],sSlygbm[60];  /*���б��� ������ݱ��� �������ر��� �����ű��� ����Ա������*/
  char sFwbsh[80]; /*�����û���ʶ�ţ� */
  /*�ļ�ָ��*/
  FILE *pmxFile;

  /*�ļ�����, �ļ��е�һ��*/
  char smxFileName[256], sLine[2048];

  fpub_InitSoComp(lXmlhandle);

#ifdef DEBUG
  LOG(LM_DEBUG,Fmtmsg("���[%s]ִ�п�ʼ",fpub_GetCompname(lXmlhandle)),"")  
#endif 

  /*��ȡ�������  --S
  COMP_PARACOUNTCHK(3)
  */
  /*ȫ�ļ���*/
  memset(smxFileName, 0, sizeof(smxFileName));

  COMP_GETPARSEPARAS(1, sXmdh, "��Ŀ����")  
  COMP_GETPARSEPARAS(2, smxFileName, "�½��ƶ�������ϸ�ļ���")
  COMP_GETPARSEPARAS(3, sDzrq, "��������")
  COMP_GETPARSEPARAS(4, sYhbm, "���б���")
  COMP_GETPARSEPARAS(5, sSldzbm, "������ݱ���")
  COMP_GETPARSEPARAS(6, sSlsxbm, "�������ر���")
  COMP_GETPARSEPARAS(7, sSlbmbm, "�����ű���")
  COMP_GETPARSEPARAS(8, sSlygbm, "����Ա������")
  iCount = 0;

  /** ��־�� 20070422 **/
  iCount = 0;
  EXEC SQL SELECT count(*) INTO :iCount
             from ywty_drls
             where xmdh =:sXmdh and zwrq=:sDzrq
               and dzbz='1' and cgbz='0' /*and jyzt='0'*/;
  if(sqlca.sqlcode != 0 || iCount <= 0)
  isXarqlsdq=__XARQLSISHIS;
  else
  isXarqlsdq=__XARQLSISCUR;
  
  LOG(LM_DEBUG,Fmtmsg("��ѯ[%s][%s]%d",sXmdh,sDzrq,isXarqlsdq),"")
  
  /*�½��ƶ�������ϸ�ļ���*/
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
  /* 20090225 ��֤����С���� sprintf(sLine, "%d\t%.2f\t%d\t%.2f\t%d\t%.2f\n", iCgzbs,dCgzje*100,iCzzbs,dCzzje*100,iCgzbs+iCzzbs,(dCgzje+dCzzje)*100); */
  sprintf(sLine, "%d\t%.0f\t%d\t%.0f\t%d\t%.0f\n", iCgzbs,dCgzje*100,iCzzbs,dCzzje*100,iCgzbs+iCzzbs,(dCgzje+dCzzje)*100); /* 20090225 ��֤����С���� */
  fputs(sLine, pmxFile);       /*������ϸ��Ϣ*/
            
  /*ȼ���û��š������·�(hcq�ֶ�)�����׽��������ڡ��������(G)�������š���ˮ�� */
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
    /*�ر��ļ�*/
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
    char sDqdm[20],sFjbz[20],sYhjfsj[80],sYdjfsj[80];  /*�û��������ݱ���;fjbz;���нɷ�ʱ��;�ƶ��ɷ�ʱ��;*/
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
    
    /* 20080131 �޸� */
    if(Sywty_drls.jyzt[0] == '0') /* �������� */
    sprintf(sLine, "%s\t%s\t%s\t%s\t%s\t%s\t%8.8s%08d\t%s\t%s\t%s\t%s\t%.0f\t%s\t%s\t%s\t%s\t%s\t%s\n",
    							 sYhbm,sSldzbm,sSlsxbm,sSlbmbm,sSlygbm,  /*���б��� ������ݱ��� �������ر��� �����ű��� ����Ա������*/
                   sDqdm,sDzrq,Sywty_drls.zhqzlsh,sYhjfsj,Sywty_drls.fkpzh,sYdjfsj, /*�û��������ݱ��� �����շ���ˮ �����շ�ʱ�䣨YYYYMMDDHH24MISS�� �ƶ��շ���ˮ �շ�ʱ�䣨YYYYMMDDHH24MISS��*/
                   sFwbsh,Sywty_drls.jyje*100,"0","0","0",/*�û���־�� ʵ�ɽ�� ���ѷ�ʽ �ɷѿ����� �ɷѿ�����*/
                   sFjbz,"0","0"); /*�Ƿ񸴻��������û���������־(������-δ����,��1��-����) ���з���ʱ�䣨YYYYMMDDHH24MISS��*/
     else /* �������� */
    sprintf(sLine, "%s\t%s\t%s\t%s\t%s\t%s\t%8.8s%08d\t%s\t%s\t%s\t%s\t%.f\t%s\t%s\t%s\t%s\t%s\t%8.8s%6.6s\n",
    							 sYhbm,sSldzbm,sSlsxbm,sSlbmbm,sSlygbm,  /*���б��� ������ݱ��� �������ر��� �����ű��� ����Ա������*/
                   sDqdm,sDzrq,Sywty_drls.zhqzlsh,sYhjfsj,Sywty_drls.fkpzh,sYdjfsj, /*�û��������ݱ��� �����շ���ˮ �����շ�ʱ�䣨YYYYMMDDHH24MISS�� �ƶ��շ���ˮ �շ�ʱ�䣨YYYYMMDDHH24MISS��*/
                   sFwbsh,Sywty_drls.jyje*100,"0","0","0",/*�û���־�� ʵ�ɽ�� ���ѷ�ʽ �ɷѿ����� �ɷѿ�����*/
                   sFjbz,"1",sDzrq,sYdjfsj); /*�Ƿ񸴻��������û���������־(������-δ����,��1��-����) ���з���ʱ�䣨YYYYMMDDHH24MISS��*/

    fputs(sLine, pmxFile);       /*������ϸ��Ϣ*/ 
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
      /*�ر��ļ�*/
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
  
  /*�ر��ļ�*/
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
  LOG(LM_DEBUG,Fmtmsg("���[%s]ִ�н���",fpub_GetCompname(lXmlhandle)),"")  
#endif
  
  return COMPRET_SUCC;
}

/* �½��ƶ����������XML�ڵ��Ӧ�Ľ�����/100 */
IRESULT XJYD_MoneyDiv(HXMLTREE lXmlhandle)
{
  char sdata[200],sbuf[200],spara[200],sFmt[254];
  int iParas,i,ilen,j;
  int iBcs,iBlws; /*��������С�������λ��*/
  fpub_InitSoComp(lXmlhandle);
  COMP_GETPARSEPARAS(1,sbuf,"������������100��")
  iBcs = atoi(sbuf);
  COMP_GETPARSEPARAS(2,sbuf,"С�������λ��")
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
    LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg("����������<3,paras[%d]",
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
 
    LOG(LM_STD,Fmtmsg("����[%s]�Ľڵ��Ӧ�Ľ��ֵ[%s]Ϊ[%s]",spara,sbuf,sdata),"");
  }
  fpub_SetCompStatus(lXmlhandle,COMPRET_SUCC);
  return COMPRET_SUCC;  	
}

/**
	�½��ƶ��������۴���   
	ʵ�ִ�ǩԼ��ϸ�� ywyy_cib_dlwtgx ��������δ�۵�ǩԼ��¼�������Զ����۽��״���    
**/
IRESULT XJYD_PLDK(HXMLTREE lXmlhandle)
{
EXEC SQL BEGIN DECLARE SECTION;
  char sJyzh[20],sKhbs[30],sZwrq[60],sPzdh[20],sQymm[10],sQkfs[10],sZhzl[10];
  char sFyrq[30],sScjysj[30]; /* ��������,�ϴν���ʱ�� */ 
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
  LOG(LM_DEBUG,Fmtmsg("���[%s]ִ�п�ʼ",fpub_GetCompname(lXmlhandle)),"")  
#endif 

  /*��ȡ�������  --S
  COMP_PARACOUNTCHK(4) */
  
  COMP_GETPARSEPARAS(1, sXmdh, "��Ŀ����")
  COMP_GETPARSEPARAS(2, sZwrq, "��������")
  trim(sZwrq);   
  COMP_GETPARSEPARAS(3,sFlow,"���ʹ��۽��״���������")
  COMP_GETPARSEPARAS(4,sProMode,"���̴���ģʽ") /*����ģʽ,����ִ�еĽ���Ƿ�Ӱ���ļ�ת������ 0 ��Ӱ��, 1 ������ִ��ʧ�� ʧ�ܽ��� 2 ������ִ��ʧ�� �ļ�ת�����Ը�ʧ��,��������,�������� */
  COMP_GETPARAS(5, sJyzhnode, "���ؽ����˺Žڵ�")
  COMP_GETPARAS(6, sKhbsnode, "���ؿͻ���ʶ�ڵ�")
  COMP_GETPARAS(7, sBslxnode, "���ر�ʶ���ͽڵ�")
  COMP_GETPARAS(8, sPzdhnode, "����ƾ֤���Žڵ�")
  /**
  COMP_GETPARAS(7, pzhqzlsh, "������ˮ��")
  COMP_GETPARAS(8, pisend, "�������һ�ʱ�ʶ")
     
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
                Fmtmsg("open cursor ʧ��[%d]",isqlcode));
         fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL);
         LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg("open cursor ʧ��[%d]",isqlcode), 
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
                Fmtmsg("fetch cursor ʧ��[%d]",isqlcode));
         fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL);
         LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg("fetch cursor ʧ��[%d]",isqlcode), 
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
      
   		/* 20090302 �������� */
   		EXEC SQL update ywyy_cib_dlwtgx set clbs='9',xyxx='��ʼ����' where ywlx=:sXmdh and jyzh=:sJyzh and khbs=:sKhbs;
   		
   		/**
   		if(strlen(sKhbs) == 8 || strlen(sKhbs) == 7)
   		sprintf(sKhbs,"0991%s",sKhbs);  �����ʶ��û�д��������Զ���������*/     
      COMP_SOFTSETXML(sKhbsnode,sKhbs)
      
     
      /*���������̽��д��۴���*/
         if ( fpub_CallFlow(lXmlhandle,sFlow) != MID_SYS_SUCC)
         {

         		EXEC SQL CLOSE cur_ywty_drls;
	 					EXEC SQL FREE cur_ywty_drls; /* 20070828*/
         fpub_SetMsg(lXmlhandle,MID_SYS_COMPEXEC,
                Fmtmsg("����ִ������[%s] ʧ��",sFlow));
         fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL);
         LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg("����ִ������[%s] ʧ��",sFlow), 
					fpub_GetCompname(lXmlhandle))
         }

         /*�ж��������Ƿ���ɹ�*/
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
                Fmtmsg("����ִ������[%s] ʧ��",sFlow));
         fpub_SetCompStatus(lXmlhandle,COMPSTATUS_FAIL);
         LOGRET(COMPRET_FAIL,LM_STD,Fmtmsg("����ִ������[%s] ʧ��",sFlow), 
				fpub_GetCompname(lXmlhandle))
				}
				else
				COMP_SOFTSETXML("/pub/respcode","00000")  /* 20080509 ��Ȼ�Ǻ�����,��ǿ�������ڲ���Ӧ��Ϊ�ɹ� */
	
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
    LOG(LM_DEBUG,Fmtmsg("���[%s]ִ�н���",fpub_GetCompname(lXmlhandle)),"")
#endif
       fpub_SetCompStatus(lXmlhandle,COMPSTATUS_SUCC);
       return COMPRET_SUCC;
}