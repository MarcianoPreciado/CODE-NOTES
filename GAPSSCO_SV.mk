#!!不要随意修改本文件
##########################################################################
##########################################################################
# 版权所有:杭州恒生电子股份有限公司
# 项目名称:综合应用平台（GAPS）项目
# 版    本:V1.0
# 操作系统:SCO5.0
# 文件描述:通用MAKEFILE	for SCO
# 数 据 库:Informix 7 
# 中 间 件:HSXP 1.0
##########################################################################
##########################################################################
include $(GAPSSRCHOME)/etc/GAPSALL.mk

#######################################
#MACRO
OS=SCO
CC=cc
LD=ld

MOS=-D__OS_SCO__
DBS=-DDBS_INFORMIX_7 -DDBS_INFORMIX

DEBUG=\
-DUSE_SHMRMA \
-DTIMEDEBUG \
-DDEBUG 
#-DDEBUG_RMA \
#-DDEBUG_FLOW \
#-DDEBUG_SYS \
#-DDEBUG_FILE \
#-DDEBUG_BILL \
#-DDEBUG_MON \
#-DDEBUG_FUNC \
#-DDEBUG_FCOMP \
#-DDEBUG_TSTRUCT \
#-DDEBUG_MEMLEAK 

E_DBS=-EDDBS_INFORMIX_7 -EDDBS_INFORMIX
E_DEBUG=\
-EDTIMEDEBUG
#-EDDEBUG \

#MACRO



#######################################
#COMPILE FLAG --S
GAPS_CCFLAGS=$(GAPS_INCL) $(GAPS_LIBINCL) ${GAPS_COMPINCL} ${GAPS_CUSTOMINCL}
INFOR_CCFLAGS=$(INFOR_INCL)
HSXP_CCFLAGS=$(HSXP_INCL)

CCFLAGS=-DBUILDDATETIME=\"$(BUILDDATETIME)\" $(GAPS_CCFLAGS) $(INFOR_CCFLAGS) $(HSXP_CCFLAGS)
#COMPILE FLAG --E
#######################################
#LINK FLAG --S
GAPS_LIBFLAG=-L$(GAPS_COMPLIB) -L$(GAPS_LIB) -L$(GAPS_CUSTOMLIB) 
INFOR_LIBFLAG=${INFOR_LIBS}
HSXP_LIBFLAG=${HSXP_LIBS}

GAPS_SOFLAG=-G

LIBFLAGS=-dy  ${GAPS_LIBFLAG} ${INFOR_LIBFLAG} ${HSXP_LIBS} 

#LINK FLAG --E
#######################################
#LIB --S
LIB_BASE=-lftp -lc -lm -lsocket -lpanel -lmenu -lform -lcurses /usr/lib/libdl.so
FIX=

#GAPS
LIB_XML=-lsxml${FIX}
LIB_HSCOMMON=-lhsetc${FIX} -lhscomm${FIX} -lhsencode${FIX} -lhsdata${FIX} 
LIB_HSUIT=-lHSUIT${FIX}
LIB_GAPS=-lgaps${FIX} -lgapssql${FIX} -lgaps${FIX}
LIB_CHANNEL=-lchannel${FIX} -lcommanz${FIX}
LIB_HSXP=-lhsxplib${FIX}

LIB_GAPSBASE=$(LIB_GAPS) $(LIB_XML) $(LIB_HSCOMMON) 

LIB_GAPSFUNC=-lgapsfunc${FIX} -lgapsfuncsys${FIX} -lgapsfuncuser${FIX} $(LIB_GAPS)
LIB_GAPSSFS=-lgapssfs${FIX} -lgapsfcompsys${FIX} -lgapsfcompuser${FIX} $(LIB_GAPS)
LIB_GAPSFUNC_0=-lgapsfunc${FIX} -lgapsfuncsys0${FIX} -lgapsfuncuser0${FIX} $(LIB_GAPS)
LIB_GAPSFUNC_SYS=-lgapsfunc${FIX} -lgapsfuncsys${FIX} -lgapsfuncuser0${FIX} $(LIB_GAPS)
LIB_GAPSSFS_0=-lgapssfs${FIX} -lgapsfcompsys0${FIX} -lgapsfcompuser0${FIX} $(LIB_GAPS)
LIB_GAPSCALC=-lgapscalc${FIX}
LIB_GAPSBILLC=-lgapsbillc${FIX}
#HSXP
LIB_HSXPXA=-lhsxpxa${FIX}
LIB_HSXPSVR=-lhsxpsvr${FIX}
LIB_HSXPLCLI=-lhsxpsvr${FIX}
LIB_HSXPWCLI=-lhsxplib${FIX} -lhsxpwcli${FIX}

LIB_INFOR=-lixsql -lixasf -lixgen -lixos -lixgls -lnsl_s -lcrypt_i /usr/lib/libgen.a -lsocket -lm -lsuds -lx -lelf -lprot -lx -ltinfo ${INFORMIXDIR}/lib/esql/checkapi.o -lixglx 
LIB_INFORXA=${INFORMIXDIR}/lib/esql/libinfxxa.a -lixsql -lixasf -lixgen -lixos -lixgls -lnsl_s -lcrypt_i -lsocket -lm -lsuds -lx ${INFORMIXDIR}/lib/esql/checkapi.o -lixglx 


#LIB --E
##########################################################################
.SUFFIXES: .ec .o 

.c.o:
	${CC} -c ${DEBUG} ${DBS} ${MOS} ${CCFLAGS}  $<

.ec.o:
	@rm -f $*.c 
	${ESQL}	-c   -O ${DEBUG} ${DBS}  ${MOS} ${E_DEBUG} ${E_DBS} ${CCFLAGS} $*.ec
	@mv -f $*.c ${GAPS_SRCTMP}
	
##########################################################################
LINK_BIN= $(CC)  $(LIBFLAGS)  -o ${GAPS_BIN}/$@ $? 

EC_LINK_COMP_SO=$(ESQL)  $(LIBFLAGS) $(GAPS_SOFLAG) -o ${GAPS_COMPSO}/$@ $? $(LIB_BASE) $(LIB_GAPSBASE) 
EC_LINK_COMMON=$(ESQL) $(LIBFLAGS) -o ${GAPS_BIN}/$@ $?  $(LIB_GAPSBASE) $(LIB_BASE)
#20061215新增
EC_LINK_SDS=$(ESQL) $(LIBFLAGS) -o ${GAPS_BIN}/$@ $?  $(LIB_GAPSSFS_0) $(LIB_GAPSBASE) $(LIB_BASE)
LINK_COMP_SO=$(CC) $(LIBFLAGS) $(GAPS_SOFLAG) -o ${GAPS_COMPSO}/$@ $? $(LIB_BASE) $(LIB_GAPSBASE) 
LINK_COMMON=$(CC) $(LIBFLAGS) -o ${GAPS_BIN}/$@ $? $(LIB_BASE) $(LIB_GAPSBASE) 
ARCMD=ar rc ${GAPS_LIB}/$@ $?
SOCMD=$(CC) $(GAPS_SOFLAG) -o ${GAPS_LIB}/$@ $?


BUILD_HSXPNOXASVR=makexpsvr -v -s$@ -o ${GAPS_BIN}/$@ -f "$? $(LIBFLAGS) $(INFOR_LIBS)  $(LIB_GAPSBASE) $(LIB_BASE) $(LIB_INFOR)" 
BUILD_HSXPXASVR=makexpsvr -v -s$@ -o ${GAPS_BIN}/$@ -f "$? $(LIBFLAGS) ${LIB_HSXPXA} $(INFOR_LIBS)  $(LIB_GAPSBASE) $(LIB_BASE) $(LIB_INFORXA)" 
BUILD_HSXPNOXAMSVR=makexpsvr -v  -o ${GAPS_BIN}/$@ -f "$?  $(LIBFLAGS) $(INFOR_LIBS)  $(LIB_GAPSBASE) $(LIB_BASE) $(LIB_INFOR)" 
BUILD_HSXPXAMSVR=makexpsvr -v  -o ${GAPS_BIN}/$@ -f "$?  $(LIBFLAGS) ${LIB_HSXPXA} $(INFOR_LIBS)  $(LIB_GAPSBASE) $(LIB_BASE) $(LIB_INFORXA)" 
LINK_HSXPSVRSO=$(CC)  $(LIBFLAGS) $(GAPS_SOFLAG) -o ${GAPS_COMPSO}/$@ $?  $(LIB_GAPSBASE) $(LIB_BASE)  ${LIB_HSXPSVR} ${LIB_HSXP}
LINK_HSXPSVRECSO=$(CC)  $(LIBFLAGS) $(INFOR_LIBS) $(GAPS_SOFLAG) -o ${GAPS_COMPSO}/$@ $? $(LIB_BASE) $(LIB_GAPSBASE) ${LIB_HSXPSVR} $(LIB_INFOR) ${LIB_HSXP}

LINK_HSXPLCLI=$(CC)  $(LIBFLAGS) -o ${GAPS_BIN}/$@ $?  $(LIB_BASE) $(LIB_GAPSBASE) ${LIB_HSXPLCLI}
LINK_HSXPWCLI=$(CC)  $(LIBFLAGS) -o ${GAPS_BIN}/$@ $?  $(LIB_BASE) $(LIB_GAPSBASE) ${LIB_HSXPWCLI}
