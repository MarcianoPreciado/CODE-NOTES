#include <stdio.h>
#include <stdlib.h>

EXEC SQL define SUCCESS 0;
EXEC SQL define WARNING 1;
EXEC SQL define NODATA 100;
EXEC SQL define RTERROR -1;
char statement[80];

int4 sqlstate_err(void)
{
    int4 err_code = RTERROR;
    if(SQLSTATE[0] == '0') { /* 处理00, 01, 02，如果是其它，则SQLERROR，直接返回 -1*/
        switch(SQLSTATE[1]) {
            case '0':
                err_code = SUCCESS;
                break;
            case '1':
                err_code = WARNING;
                break;
            case '2':
                err_code = NODATA;
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
EXEC SQL BEGIN DECLARE SECTION;
    mint exception_count;
    char overflow[2];
    int exception_num = 1;
    char class_id[255];
    char subclass_id[255];
    char message[8191];
    mint messlen;
    char sqlstate_code[6];
    mint i;
EXEC SQL END DECLARE SECTION;
    printf("-------------------------------------------------------------\n");
    printf("SQLSTATE: %s\n", SQLSTATE);
    printf("SQLCODE: %d\n", SQLCODE);
    printf("\n");
    
    EXEC SQL GET DIAGNOSTICS :exception_count = NUMBER,
         :overflow = MORE;
    printf("EXCEPTIONS: Number=%d\t", exception_count);
    printf("More? %s\n", overflow);
    for (i =1; i <= exception_count; i++) {
        EXEC SQL GET DIAGNOSTICS exception :i
            :sqlstate_code = RETURNED_SQLSTATE,
            :class_id = CLASS_ORIGIN, :subclass_id = SUBCLASS_ORIGIN,
            :message = MESSAGE_TEXT, :messlen = MESSAGE_LENGTH;
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
        case SUCCESS:
        case NODATA:
            break;
        case WARNING:
            if (warn_flg)
                disp_warning(stmt);
            break;
        case RTERROR:
            disp_error(stmt);
            break;
        default:
            printf("\n**********INVALID EXCEPTION STATE FOR %s*****\n",stmt);
            break;
    }
}

int4 exp_chk2(char *stmt, mint warn_flg)
{
    int4 sqlerr_code = SUCCESS;
    int4 sqlcode;
    sqlcode = SQLCODE;
    sqlerr_code = sqlstate_err();
    disp_exception(stmt, sqlerr_code, warn_flg);
    if(sqlerr_code = RTERROR)
        sqlerr_code = sqlcode;
    return sqlerr_code;
}

int whenexp_chk(void)
{
    int4 sqlerr_code = SUCCESS;
    mint disp = 0;
    sqlerr_code = sqlstate_err();
    if(sqlerr_code == WARNING) {
        disp = 1;
        printf("\n************Warning encountered in\
        %s***********\n",statement);
    }
    else if(sqlerr_code == RTERROR) {
        disp = 1;
        printf("\n************Error encountered in           %s***********\n",statement);
    }
    if(disp)
        disp_sqlstate_err();
    if(sqlerr_code == RTERROR) {
        /* Exit the program after examining the error */
        printf("**************Program terminated****************\n\n");
        exit(1);
    }
    else if(sqlerr_code == WARNING) {
        printf("\n***************Program execution continues*******\n\n");
        return sqlerr_code;
    }
    return 0;
}

