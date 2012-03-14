#include <stdio.h>
#include <string.h>

EXEC SQL include sqlca;
EXEC SQL define FNAME_LEN 15;
EXEC SQL define LNAME_LEN 15;

int4 sqlstate_err();
extern char statement[80];

int main(void)
{
    EXEC SQL BEGIN DECLARE SECTION;
    char fname[FNAME_LEN + 1];
    char lname[LNAME_LEN + 1];
    EXEC SQL END DECLARE SECTION;

    EXEC SQL WHENEVER SQLERROR CALL whenexp_chk;
    EXEC SQL WHENEVER SQLWARNING CALL whenexp_chk;
    printf("GETDIAG Sample ESQL program running.\n\n");
    strcpy(statement, "CONNECT stmt");
    EXEC SQL DATABASE 'stores7';
    strcpy(statement, "DCCLARE stmt");
    EXEC SQL DECLARE democursor CURSOR for
        SELECT fname, lname
        into :fname, :lname
        from customer
        where lname < 'D';
    strcpy(statement, "OPEN stmt");
    EXEC SQL OPEN democursor;
    strcpy(statement, "FETCH stmt");
    for (; ;) {
        EXEC SQL FETCH democursor;
        if(sqlstate_err() == 100)
            break;
        printf("%s %s\n", fname, lname);
    }
    strcpy(statement, "CLOSE stmt");
    EXEC SQL CLOSE democursor;
    strcpy(statement, "FREE stmt");
    EXEC SQL FREE democursor;
    strcpy(statement, "DISCONNECT stmt");
    EXEC SQL DISCONNECT CURRENT;
    printf("\nGETDIAG Sample Program Over.\n");
}
EXEC SQL include exp_chk.ec;


