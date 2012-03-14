#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <decimal.h>
#include <errno.h>

EXEC SQL include sqltypes;
#define LCASE(c) (isupper(c) ? tolower(c) : (c));

/* Defines for callback mechanism */
#define DB_TIMEOUT      200     /*number of milliseconds in timeout */
#define SQL_INTERRUPT   -213    /* 被中断的SQLCODE 值 */

#define MAX_ROWS        10000   /* 被添加到表中的行数 */
EXEC SQL define CHARLDSIZE  20  /* 表中字符列的大小 */
#define SERVER_BUSY     -439
#define WARNNOTIFY      1
#define NOWARNNOTIFY    0
int4 dspquery();
extern int4 exp_chk2();
void on_timeout();

int main(void)
{
    char ques[80], prompt_ans();
    int4 ret;
    mint create_tbl(), drop_tbl();
    printf("TIMEOUT Sample ESQL Program running\n");
    EXEC SQL CONNECT to 'stores7';

    if (exp_chk2("CONNECT to stores7", NOWARNNOTIFY) < 0)
        exit(1);
    printf("Connected to 'stores7' on default server\n");

    if(!create_tbl()) {
        printf("\nTIMEOUT Sample Program over.\n\n");
        exit(1);
    }
    for(; ;) {
        if(sqlbreakcallback(DB_TIMEOUT, on_timeout)) {
            printf("\nUnable to establish callback function.\n");
            printf("TIMEOUT Sample Program over.\n\n");
            exit(1);
        }
        printf("Timeout interval for SQL request is: ");
        printf("%0.8f minuetes\n", DB_TIMEOUT/60000.00);
        stcopy("Are you ready to begin execution of the query?", ques);
        if(prompt_ans(ques) == 'n') {
            /* Unregister callback function so table cleanup will no be
             * interrupted. */
            sqlbreakcallback(-1L, (void *)NULL);
            break;
        }
        printf("\nBeginning execution of query...\n\n");
        if((ret = dspquery()) == 0) {
            if(prompt_ans("Try another run?") == 'y')
                continue;
            else
                break;
        }
        else
            exit(1);
    }
    drop_tbl();
    EXEC SQL DISCONNECT CURRENT;
    if(exp_chk2("DISCONNECT for stores7", WARNNOTIFY) != 0)
        exit(1);
    printf("\nDisconnected stores7 connection\n");
    printf("\nTIMEOUT Sample Program over.\n\n");
}

int4 dspquery()
{
    mint cnt = 0;
    int4 ret = 0;
    int4 sqlcode = 0;
    int4 sqlerr_code, sqlstate_err();
    void disp_exception(), disp_error(), disp_warning();
    EXEC SQL BEGIN DECLARE SECTION;
        char fld1_val[CHARFLDSIZE + 1];
        char fld2_val[CHARFLDSIZE + 1];
        int4 int_val;
    EXEC SQL END DECLARE SECTION;

    EXEC SQL DECLARE cancel_curs CURSOR for
        select sum(int_fld), char_fld1, char_fld2
            from canceltst
            where char_fld1 matches "*f*"
                or char_fld1 matches "*h*"
                or char_fld2 matches "*w*"
                or char_fld2 matches "*l*"
            GROUP BY char_fld1, char_fld2;

    EXEC SQL OPEN cancel_curs;
    sqlcode = SQLSTATE;
    sqlerr_code = sqlstate_err();
    if(sqlerr_code != 0) {
        if(sqlerr_code == -1) {
            if(sqlcode == SQL_INTERRUPT) {
                printf("\n      TIMEOUT INTERRUPT PROCESSED\n\n");
                sqlcode = 0;
            }
            else
                disp_error("OPEN cancel_curs");
            EXEC SQL CLOSE cancel_curs;
            EXEC SQL FREE cancel_curs;
            return sqlcode;
        }
        else if(sqlerr_code == 1)
            disp_warning("OPEN cancel_curs");
    }
    printf("Displaying data..........\n");
    while(1) {
        EXEC SQL FETCH cancel_curs into :int_val, :fld1_val, :fld2_val;
        if ((ret = exp_chk2("FETCH from cancel_curs", NOWARNNOTIFY)) == 0)
        {
            printf("sum(int_fld) = %d\n", int_val);
            printf("char_fld1 = %s\n", fld1_val);
            printf("char_fld2 = %s\n\n", fld2_val);
        }
        else{
/*
* Will display warning messages (WARNNOTIFY) but continue
* execution when they occur (exp_chk2() == 1)
*/
            if (ret==100) {
/* NOT FOUND condition */
                printf("\nNumber of rows found: %d\n\n", cnt);
                break;
            }
            if (ret < 0) {
/* Runtime error */
                EXEC SQL close cancel_curs;
                EXEC SQL free cancel_curs;
                return(ret);
            }
        }
        cnt++;
} /* end whille */


