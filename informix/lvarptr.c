#include <sqlhdr.h>
#include <sqliapi.h>
static const char _Cn2[] = "c_collect";
static const char _Cn1[] = "p_collect";
#line 1 "lvarptr.ec"
/****************************************************************************
 * Licensed Material - Property Of IBM
 *
 * "Restricted Materials of IBM"
 *
 * IBM Informix Client SDK
 *
 * (c)  Copyright IBM Corporation 1997, 2008. All rights reserved.
 *
 ****************************************************************************
 */
/*
**
** Sample use of LVARCHAR to fetch collections in ESQL/C.
**
** Statically determined collection types.
*/
/* 
 * EXEC SQL include "exp_chk.ec";
 */
#line 18 "lvarptr.ec"

#line 18 "lvarptr.ec"
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
        printf("\n************Error encountered in           %s***********\n",statement);
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

#line 144 "exp_chk.ec"
#line 19 "lvarptr.ec"
#include <stdio.h>
static void print_lvarchar_ptr(
const char *tag,
/*
 * EXEC SQL BEGIN DECLARE SECTION;
 */
#line 22 "lvarptr.ec"
#line 23 "lvarptr.ec"
  void **lv
/*
 * EXEC SQL END DECLARE SECTION;
 */
#line 24 "lvarptr.ec"

)
{
	char *data;

	data = ifx_var_getdata(lv);
	if (data == 0)
		data = "<<NO DATA>>";
	printf("%s: %s\n", tag, data);
}

static void process_stmt(char *stmt)
{
/*
 * 	EXEC SQL BEGIN DECLARE SECTION;
 */
#line 37 "lvarptr.ec"
#line 38 "lvarptr.ec"
  void *lv1 = 0;
  void *lv2 = 0;
  void *lv3 = 0;
mint seq;
  char *stmt1 = stmt;
/*
 * 	EXEC SQL END DECLARE SECTION;
 */
#line 43 "lvarptr.ec"


	printf("SQL: %s\n", stmt);

/*
 * 	EXEC SQL WHENEVER SQLERROR CALL whenexp_chk;
 */
#line 47 "lvarptr.ec"
/*
 * 	EXEC SQL WHENEVER SQLWARNING CALL whenexp_chk;
 */
#line 48 "lvarptr.ec"
/*
 * 	EXEC SQL PREPARE p_collect FROM :stmt1;
 */
#line 49 "lvarptr.ec"
  {
#line 49 "lvarptr.ec"
  sqli_prep(ESQLINTVERSION, _Cn1, stmt1,(ifx_literal_t *)0, (ifx_namelist_t *)0, -1, 0, 0 ); 
#line 49 "lvarptr.ec"
  { if (SQLCODE < 0) whenexp_chk(); }
#line 49 "lvarptr.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 49 "lvarptr.ec"
}
/*
 * 	EXEC SQL DECLARE c_collect CURSOR FOR p_collect;
 */
#line 50 "lvarptr.ec"
{
#line 50 "lvarptr.ec"
sqli_curs_decl_dynm(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn2, 512), _Cn2, sqli_curs_locate(ESQLINTVERSION, _Cn1, 513), 0, 0);
#line 50 "lvarptr.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 50 "lvarptr.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 50 "lvarptr.ec"
}
/*
 * 	EXEC SQL OPEN c_collect;
 */
#line 51 "lvarptr.ec"
{
#line 51 "lvarptr.ec"
sqli_curs_open(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn2, 768), (ifx_sqlda_t *)0, (char *)0, (struct value *)0, 0, 0);
#line 51 "lvarptr.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 51 "lvarptr.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 51 "lvarptr.ec"
}


	ifx_var_flag(&lv1, 1);
	ifx_var_flag(&lv2, 1);
	ifx_var_flag(&lv3, 1);
	while (sqlca.sqlcode == 0)
	{
/*
 * 		EXEC SQL FETCH c_collect INTO :seq, :lv1, :lv2, :lv3;
 */
#line 59 "lvarptr.ec"
{
#line 59 "lvarptr.ec"
static ifx_sqlvar_t _sqobind[] = 
{
{ 102, sizeof(seq), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 124, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 124, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 124, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
#line 59 "lvarptr.ec"
};
static ifx_sqlda_t _SD0 = { 4, _sqobind, {0}, 4, 0 };
static _FetchSpec _FS1 = { 0, 1, 1 };
#line 59 "lvarptr.ec"
_sqobind[0].sqldata = (char *) &seq;
#line 59 "lvarptr.ec"
ifx_var_init(&lv1);
_sqobind[1].sqldata = (char *) lv1;
#line 59 "lvarptr.ec"
ifx_var_init(&lv2);
_sqobind[2].sqldata = (char *) lv2;
#line 59 "lvarptr.ec"
ifx_var_init(&lv3);
_sqobind[3].sqldata = (char *) lv3;
sqli_curs_fetch(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn2, 768), (ifx_sqlda_t *)0, &_SD0, (char *)0, &_FS1);
#line 59 "lvarptr.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 59 "lvarptr.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 59 "lvarptr.ec"
}
		if (sqlca.sqlcode == 0)
		{
			printf("Sequence: %d\n", seq);
			print_lvarchar_ptr("LVARCHAR 1", &lv1);
			print_lvarchar_ptr("LVARCHAR 2", &lv2);
			print_lvarchar_ptr("LVARCHAR 3", &lv3);
            printf("\n\n\n");
		}
	}
			ifx_var_dealloc(&lv1);
			ifx_var_dealloc(&lv2);
			ifx_var_dealloc(&lv3);

/*
 * 	EXEC SQL CLOSE c_collect;
 */
#line 73 "lvarptr.ec"
{
#line 73 "lvarptr.ec"
sqli_curs_close(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn2, 768));
#line 73 "lvarptr.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 73 "lvarptr.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 73 "lvarptr.ec"
}
/*
 * 	EXEC SQL FREE c_collect;
 */
#line 74 "lvarptr.ec"
{
#line 74 "lvarptr.ec"
  sqli_curs_free(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn2, 770));
#line 74 "lvarptr.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 74 "lvarptr.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 74 "lvarptr.ec"
}
/*
 * 	EXEC SQL FREE p_collect;
 */
#line 75 "lvarptr.ec"
{
#line 75 "lvarptr.ec"
  sqli_curs_free(ESQLINTVERSION, sqli_curs_locate(ESQLINTVERSION, _Cn1, 770));
#line 75 "lvarptr.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 75 "lvarptr.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 75 "lvarptr.ec"
}
}

mint main(int argc, char **argv)
{
/*
 * 	EXEC SQL BEGIN DECLARE SECTION;
 */
#line 80 "lvarptr.ec"
#line 81 "lvarptr.ec"
  char *dbase = "stores_demo";
  char *stmt1 = "INSERT INTO t_collections VALUES(0, "
		"'LIST{-1,0,-2,3,0,0,32767,249}', 'SET{-1,0,-2,3}', "
		"'MULTISET{-1,0,0,-2,3,0}') ";
  char *data = "stores7@ol_informix";
/*
 * 	EXEC SQL END DECLARE SECTION;
 */
#line 87 "lvarptr.ec"


	if (argc > 1)
		dbase = argv[1];
	printf("Connect to %s\n", dbase);
/*
 * 	EXEC SQL CONNECT TO :dbase;
 */
#line 92 "lvarptr.ec"
{
#line 92 "lvarptr.ec"
sqli_connect_open(ESQLINTVERSION, 0, dbase, (char *)0, (ifx_conn_t *)0, 0);
#line 92 "lvarptr.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 92 "lvarptr.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 92 "lvarptr.ec"
}

/*
 * 	EXEC SQL CREATE TEMP TABLE t_collections
 * 	(
 * 		seq	serial not null,
 * 		l1 list    (integer  not null),
 * 		s1 set     (integer  not null),
 * 		m1 multiset(integer  not null)
 * 	);
 */
#line 94 "lvarptr.ec"
{
#line 100 "lvarptr.ec"
static const char *sqlcmdtxt[] =
#line 100 "lvarptr.ec"
{
#line 100 "lvarptr.ec"
"CREATE TEMP TABLE t_collections ( seq serial not null , l1 list ( integer not null ) , s1 set ( integer not null ) , m1 multiset ( integer not null ) )",
0
};
#line 100 "lvarptr.ec"
static ifx_statement_t _SQ0 = {0};
#line 100 "lvarptr.ec"
sqli_stmt(ESQLINTVERSION, &_SQ0, sqlcmdtxt, 0, (ifx_sqlvar_t *)0, (struct value *)0, (ifx_literal_t *)0, (ifx_namelist_t *)0, (ifx_cursor_t *)0, -1, 0, 0);
#line 100 "lvarptr.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 100 "lvarptr.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 100 "lvarptr.ec"
}

/*
 * 	EXEC SQL EXECUTE IMMEDIATE :stmt1;
 */
#line 102 "lvarptr.ec"
{
#line 102 "lvarptr.ec"
sqli_exec_immed(stmt1);
#line 102 "lvarptr.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 102 "lvarptr.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 102 "lvarptr.ec"
}
/*
 * 	EXEC SQL EXECUTE IMMEDIATE :stmt1;
 */
#line 103 "lvarptr.ec"
{
#line 103 "lvarptr.ec"
sqli_exec_immed(stmt1);
#line 103 "lvarptr.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 103 "lvarptr.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 103 "lvarptr.ec"
}
/*
 * 	EXEC SQL EXECUTE IMMEDIATE :stmt1;
 */
#line 104 "lvarptr.ec"
{
#line 104 "lvarptr.ec"
sqli_exec_immed(stmt1);
#line 104 "lvarptr.ec"
{ if (SQLCODE < 0) whenexp_chk(); }
#line 104 "lvarptr.ec"
{ if (sqlca.sqlwarn.sqlwarn0 == 'W') whenexp_chk(); }
#line 104 "lvarptr.ec"
}

	process_stmt("SELECT seq, l1, s1, m1 FROM t_collections");

	puts("OK");
	return 0;
}



#line 112 "lvarptr.ec"
