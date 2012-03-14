#include <sqlhdr.h>
#include <sqliapi.h>
static const char _Cn1[] = "democursor";
#line 1 "getdiag.ec"
#include <stdio.h>
#include <string.h>

/* 
 * EXEC SQL include sqlca;
 */
#line 4 "getdiag.ec"

#line 4 "getdiag.ec"
#line 1 "/home/informix/informix/incl/esql/sqlca.h"
/****************************************************************************
 *
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 * Copyright IBM Corporation 1997, 2008. All rights reserved.
 *
 *  Title:       sqlca.h
 *  Description: SQL Control Area
 *
 ***************************************************************************
 */

#ifndef SQLCA_INCL
#define SQLCA_INCL

#include "ifxtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sqlca_s
    {
    int4 sqlcode;
#ifdef DB2CLI
    char sqlerrm[600]; /* error message parameters */
#else /* DB2CLI */
    char sqlerrm[72]; /* error message parameters */
#endif /* DB2CLI */
    char sqlerrp[8];
    int4 sqlerrd[6];
		    /* 0 - estimated number of rows returned */
		    /* 1 - serial value after insert or  ISAM error code */
		    /* 2 - number of rows processed */
		    /* 3 - estimated cost */
		    /* 4 - offset of the error into the SQL statement */
		    /* 5 - rowid after insert  */
#ifdef _FGL_
    char sqlawarn[8];
#else
    struct sqlcaw_s
	{
	char sqlwarn0; /* = W if any of sqlwarn[1-7] = W */
	char sqlwarn1; /* = W if any truncation occurred or
				database has transactions or
			        no privileges revoked */
	char sqlwarn2; /* = W if a null value returned or
				ANSI database */
	char sqlwarn3; /* = W if no. in select list != no. in into list or
				turbo backend or no privileges granted */
	char sqlwarn4; /* = W if no where clause on prepared update, delete or
				incompatible float format */
	char sqlwarn5; /* = W if non-ANSI statement */
	char sqlwarn6; /* = W if server is in data replication secondary mode */
	char sqlwarn7; /* = W if database locale is different from proc_locale
                          = W if backend XPS and if explain avoid_execute is set
                              (for select, insert, delete and update only)
			*/
	} sqlwarn;
#endif
    } ifx_sqlca_t;

/* NOTE: 4gl assumes that the sqlwarn structure can be defined as
 *	sqlawarn -- an 8 character string, because single-char
 *	variables are not recognized in 4gl.
 *
 * If this structure should change, the code generated by 4gl compiler
 *	must also change
 */

#define SQLNOTFOUND 100

#ifndef IFX_THREAD
#ifdef DB2CLI
#define sqlca ifmxsqlca
extern struct sqlca_s sqlca;
#else /* DB2CLI */
extern struct sqlca_s sqlca;
#endif /* DB2CLI */

#ifndef DRDAHELP
extern int4 SQLCODE;
#endif

extern char SQLSTATE[];
#else /* IFX_THREAD */
extern int4 * ifx_sqlcode(void);
extern struct sqlca_s * ifx_sqlca(void);
/* ifx_sqlstate() declared in sqlhdr.h */
#define SQLCODE (*(ifx_sqlcode()))
#define SQLSTATE ((char *)(ifx_sqlstate()))
#define sqlca (*(ifx_sqlca()))
#endif /* IFX_THREAD */

#ifdef __cplusplus
}
#endif

#endif /* SQLCA_INCL */

#line 103 "/home/informix/informix/incl/esql/sqlca.h"
/* 
 * EXEC SQL define FNAME_LEN 15;
 */
#line 5 "getdiag.ec"

/* 
 * EXEC SQL define LNAME_LEN 15;
 */
#line 6 "getdiag.ec"

#line 7 "getdiag.ec"

int4 sqlstate_err();
extern char statement[80];

int main(void)
{
/*
 *     EXEC SQL BEGIN DECLARE SECTION;
 */
#line 13 "getdiag.ec"
#line 14 "getdiag.ec"
  char fname[16];
  char lname[16];
/*
 *     EXEC SQL END DECLARE SECTION;
 */
#line 16 "getdiag.ec"


/*
 *     EXEC SQL WHENEVER SQLERROR CALL whenexp_chk;
 */
#line 18 "getdiag.ec"
/*
 *     EXEC SQL WHENEVER SQLWARNING CALL whenexp_chk;
 */
#line 19 "getdiag.ec"
    printf("GETDIAG Sample ESQL program running.\n\n");
    strcpy(statement, "CONNECT stmt");
/*
 *     EXEC SQL DATABASE 'stores7';
 */
#line 22 "getdiag.ec"
  {
#line 22 "getdiag.ec"
  sqli_db_open("stores7", 0);
#line 22 "getdiag.ec"
  { if (SQLCODE < 0) whenexp_chk(); }
#line 22 "getdiag.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 22 "getdiag.ec"
}
    strcpy(statement, "DCCLARE stmt");
/*
 *     EXEC SQL DECLARE democursor CURSOR for
 *         SELECT fname, lname
 *         into :fname, :lname
 *         from customer
 *         where lname < 'D';
 */
#line 24 "getdiag.ec"
{
#line 28 "getdiag.ec"
static const char *sqlcmdtxt[] =
#line 28 "getdiag.ec"
{
#line 28 "getdiag.ec"
"SELECT fname , lname from customer where lname < 'D'",
0
};
#line 28 "getdiag.ec"
static ifx_sqlvar_t _sqobind[] = 
{
  { 100, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 100, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
#line 28 "getdiag.ec"
};
static ifx_sqlda_t _SD0 = { 2, _sqobind, {0}, 2, 0 };
#line 28 "getdiag.ec"
_sqobind[0].sqldata = fname;
#line 28 "getdiag.ec"
_sqobind[1].sqldata = lname;
#line 28 "getdiag.ec"
sqli_curs_decl_stat(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn1, 512), _Cn1, sqlcmdtxt, (ifx_sqlda_t *)0, &_SD0, 0, (ifx_literal_t *)0, (ifx_namelist_t *)0, 2, 0, 0);
#line 28 "getdiag.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 28 "getdiag.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 28 "getdiag.ec"
}
    strcpy(statement, "OPEN stmt");
/*
 *     EXEC SQL OPEN democursor;
 */
#line 30 "getdiag.ec"
{
#line 30 "getdiag.ec"
sqli_curs_open(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn1, 768), (ifx_sqlda_t *)0, (char *)0, (struct value *)0, 0, 0);
#line 30 "getdiag.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 30 "getdiag.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 30 "getdiag.ec"
}
    strcpy(statement, "FETCH stmt");
    for (; ;) {
/*
 *         EXEC SQL FETCH democursor;
 */
#line 33 "getdiag.ec"
{
#line 33 "getdiag.ec"
static _FetchSpec _FS0 = { 0, 1, 0 };
sqli_curs_fetch(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn1, 768), (ifx_sqlda_t *)0, (ifx_sqlda_t *)0, (char *)0, &_FS0);
#line 33 "getdiag.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 33 "getdiag.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 33 "getdiag.ec"
}
        if(sqlstate_err() == 100)
            break;
        printf("%s %s\n", fname, lname);
    }
    strcpy(statement, "CLOSE stmt");
/*
 *     EXEC SQL CLOSE democursor;
 */
#line 39 "getdiag.ec"
{
#line 39 "getdiag.ec"
sqli_curs_close(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn1, 768));
#line 39 "getdiag.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 39 "getdiag.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 39 "getdiag.ec"
}
    strcpy(statement, "FREE stmt");
/*
 *     EXEC SQL FREE democursor;
 */
#line 41 "getdiag.ec"
{
#line 41 "getdiag.ec"
  sqli_curs_free(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn1, 770));
#line 41 "getdiag.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 41 "getdiag.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 41 "getdiag.ec"
}
    strcpy(statement, "DISCONNECT stmt");
/*
 *     EXEC SQL DISCONNECT CURRENT;
 */
#line 43 "getdiag.ec"
{
#line 43 "getdiag.ec"
sqli_connect_close(3, (char *)0, 0, 0);
#line 43 "getdiag.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 43 "getdiag.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 43 "getdiag.ec"
}
    printf("\nGETDIAG Sample Program Over.\n");
}
/* 
 * EXEC SQL include exp_chk.ec;
 */
#line 46 "getdiag.ec"

#line 46 "getdiag.ec"
#line 1 "exp_chk.ec"
#include <stdio.h>
#include <stdlib.h>

/* 
 * EXEC SQL define SUCCESS 0;
 */
#line 4 "exp_chk.ec"

/* 
 * EXEC SQL define WARNING 1;
 */
#line 5 "exp_chk.ec"

/* 
 * EXEC SQL define NODATA 100;
 */
#line 6 "exp_chk.ec"

/* 
 * EXEC SQL define RTERROR -1;
 */
#line 7 "exp_chk.ec"

char statement[80];

int4 sqlstate_err(void)
{
    int4 err_code = -1;
    if(SQLSTATE[0] == '0') { /* 处理00, 01, 02，如果是其它，则SQLERROR，直接返回 -1*/
        switch(SQLSTATE[1]) {
            case '0':
                err_code = 0;
                break;
            case '1':
                err_code = 1;
                break;
            case '2':
                err_code = 100;
                break;
            default: /* error, return -1 */
                break;
        }
    }
    return err_code;
}

/* disp_sqlstate_err()函数执行GET DIAGNOSTICS语句并打印所返回异常的详细信息*/
void disp_sqlstate_err(void)
{
    mint j;
/*
 * EXEC SQL BEGIN DECLARE SECTION;
 */
#line 35 "exp_chk.ec"
#line 36 "exp_chk.ec"
mint exception_count;
  char overflow[2];
int exception_num = 1;
  char class_id[255];
  char subclass_id[255];
  char message[8191];
mint messlen;
  char sqlstate_code[6];
mint i;
/*
 * EXEC SQL END DECLARE SECTION;
 */
#line 45 "exp_chk.ec"

    printf("-------------------------------------------------------------\n");
    printf("SQLSTATE: %s\n", SQLSTATE);
    printf("SQLCODE: %d\n", SQLCODE);
    printf("\n");

/*
 *     EXEC SQL GET DIAGNOSTICS :exception_count = NUMBER,
 *          :overflow = MORE;
 */
#line 51 "exp_chk.ec"
{
#line 52 "exp_chk.ec"
static ifx_hostvar_t _SQhtab[] =
{
{ 0, 1, 102, sizeof(exception_count), 0, 0, 0, 0 },
{ 0, 2, 100, 2, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 }
#line 52 "exp_chk.ec"
};
_SQhtab[0].hostaddr = (char *)&exception_count;
_SQhtab[1].hostaddr = (overflow);
#line 52 "exp_chk.ec"
sqli_diag_get(ESQLINTVERSION, _SQhtab, -1);
#line 52 "exp_chk.ec"
}
    printf("EXCEPTIONS: Number=%d\t", exception_count);
    printf("More? %s\n", overflow);
    for (i =1; i <= exception_count; i++) {
/*
 *         EXEC SQL GET DIAGNOSTICS exception :i
 *             :sqlstate_code = RETURNED_SQLSTATE,
 *             :class_id = CLASS_ORIGIN, :subclass_id = SUBCLASS_ORIGIN,
 *             :message = MESSAGE_TEXT, :messlen = MESSAGE_LENGTH;
 */
#line 56 "exp_chk.ec"
{
#line 59 "exp_chk.ec"
static ifx_hostvar_t _SQhtab[] =
{
{ 0, 3, 100, 6, 0, 0, 0, 0 },
{ 0, 4, 100, 255, 0, 0, 0, 0 },
{ 0, 5, 100, 255, 0, 0, 0, 0 },
{ 0, 6, 100, 8191, 0, 0, 0, 0 },
{ 0, 7, 102, sizeof(messlen), 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 }
#line 59 "exp_chk.ec"
};
_SQhtab[0].hostaddr = (sqlstate_code);
_SQhtab[1].hostaddr = (class_id);
_SQhtab[2].hostaddr = (subclass_id);
_SQhtab[3].hostaddr = (message);
_SQhtab[4].hostaddr = (char *)&messlen;
#line 59 "exp_chk.ec"
sqli_diag_get(ESQLINTVERSION, _SQhtab, i);
#line 59 "exp_chk.ec"
}
        printf("--------------------------------------------------\n");
        printf("EXCEPTION %d, SQLSTATE = %s\n", i, sqlstate_code);
        message[messlen - 1] = '\0';
        printf("MESSAGE TEXT: %s\n", message);
        j = byleng(class_id, stleng(class_id));
        class_id[j] = '\0';
        printf("CLASS ORIGIN: %s\n", class_id);
        j = byleng(subclass_id, stleng(subclass_id));
        subclass_id[j] = '\0';
        printf("SUBCLASS ORIGIN: %s\n", class_id);
    }
    printf("-------------------------------------------------------------\n");
}

void disp_error(char *stmt)
{
    printf("\n**************Error encountered in %s**********\n", stmt);
    disp_sqlstate_err();
}

void disp_warning(char *stmt)
{
    printf("\n***************Warning encountered in %s********\n", stmt);
    disp_sqlstate_err();
}

void disp_exception(char *stmt, int4 sqlerr_code, mint warn_flg)
{
    switch(sqlerr_code) {
        case 0:
        case 100:
            break;
        case 1:
            if (warn_flg)
                disp_warning(stmt);
            break;
        case -1:
            disp_error(stmt);
            break;
        default:
            printf("\n**********INVALID EXCEPTION STATE FOR %s*****\n",stmt);
            break;
    }
}

int4 exp_chk2(char *stmt, mint warn_flg)
{
    int4 sqlerr_code = 0;
    int4 sqlcode;
    sqlcode = SQLCODE;
    sqlerr_code = sqlstate_err();
    disp_exception(stmt, sqlerr_code, warn_flg);
    if(sqlerr_code = -1)
        sqlerr_code = sqlcode;
    return sqlerr_code;
}

int whenexp_chk(void)
{
    int4 sqlerr_code = 0;
    mint disp = 0;
    sqlerr_code = sqlstate_err();
    if(sqlerr_code == 1) {
        disp = 1;
        printf("\n************Warning encountered in\
               %s***********\n",statement);
    }
    else if(sqlerr_code == -1) {
        disp = 1;
        printf("\n************Error encountered in\
               %s***********\n",statement);
    }
    if(disp)
        disp_sqlstate_err();
    if(sqlerr_code == -1) {
        /* Exit the program after examining the error */
        printf("**************Program terminated****************\n\n");
        exit(1);
    }
    else if(sqlerr_code == 1) {
        printf("\n***************Program execution continues*******\n\n");
        return sqlerr_code;
    }
    return 0;
}



#line 147 "exp_chk.ec"
#line 47 "getdiag.ec"



#line 48 "getdiag.ec"