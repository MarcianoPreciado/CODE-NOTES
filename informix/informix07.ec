EXEC SQL BEGIN DECLARE SECTION;
mint exception_count;
char overflow[2];
EXEC SQL END DECLARE SECTION;
.
.
EXEC SQL create database db;
EXEC SQL create table tab1 (col1 integer);
EXEC SQL get diagnostics :exception_count = NUMBER,
:overflow = MORE;
/* Figure 11-1. Using GET DIAGNOSTICS to Return Statement Information */
Filed-Name Keyword      ESQL/C DATA TYPE        Description
NUMBER                  mint                   诊断区最近接受到的异常数量，
                                        1-35000， 当SQL语句执行成功，数量就增加1
MORE                    char[2]         "N"或"Y"，超出能存储的异常数，则为Y。
RAW_COUNT               mint            INSERT/UPDATE/DELETE成功的总数。

/* GET DIAGNOTICS语句返回的异常信息。*/

RETURNED_SQLSTATE   char[6]     当前异常的SQLSTATE值。
INFORMIX_SQLCODE    int4        Informix指定的状态信息。也设置全局SQLCODE值。

If Informix defines the class (subclass) code, the value is "IX000". If the
International Standards Organization(ISO) defines the class code, the value of
CLASS_ORIGIN is "ISO 9075".If a user-defined routine has
defined the message text of the exception, the value is "U0001".

CLASS_ORIGIN        char[255]   如果Informix定义了消息类，职位IX000，如果ISO
标准定义了消息类，值为"ISO 9075"，如果用户自定义了异常消息，值为"U0001"

SUBCLASS_ORIGIN     char[255]   消息子类，值同上。

MESSAGE_TEXT        char[8191]  可变长数组存储。包括异常信息情况，ISAM异常或者
用户定义的信息。
MESSAGE_LENGTH      mint        MESSAGE_TEXT所包含的信息长度。
SERVER_NAME         char[255]   当前CONNECT or DATABASE 的数据库服务器名。
CONNECTION_NAME     char[255]   可变长数组，with the actions of a CONNECT or
SET CONNECTION 语句的客户端名字。

EXEC SQL BEGIN DECLARE SECTION;
char class_origin_val[255];
char subclass_origin_val[255];
char message_text_val[8191];
mint messlength_val;
EXEC SQL END DECLARE SECTION;

EXEC SQL create database db;
EXEC SQL create table tab1 (col1 integer);
EXEC SQL get diagnostics exception 1
:class_origin_val = CLASS_ORIGIN,
:subclass_origin_val = SUBCLASS_ORIGIN,
:message_text_val = MESSAGE_TEXT,
:messlength_val = MESSAGE_LENGTH;

EXEC SQL get diagnostics exception 1 :SQLSTATE = RETURNED_SQLSTATE;
运行时Informix自动将diagnositics区域中的RETURNED_SQLSTATE值拷贝到全局变量SQLSTATE。

SQLSTATE有5个字符，前两个为类代码值，后三个为子类代码值。

CLASS CODE值首字母    异常代码定义者          说明
0-4 A-H         X/Open与ANSI/ISO定义    子类代码仍以0-4 A-H开头
5-9             依定义实现              子类也依定义实现
I-Z             Informix ESQL/C定义     

"IX000"         运行时错误，Informix指定错误消息
"IX001"         运行时错误，GET DIAGNOSTICS语句错误
"U0001"         运行时错误，用户自定义
"01Ixx"         警告，Informix指定
"01U01"         警告，用户自定义


SQLSTATE返回的值：

/*异常情况          值                        */

成功                "00000"     (可以只检测class code就确定成功)
成功，但没找到行    "02000"     (可以只检测class code就确定成功)
成功，但有警告      类代码为"01"，子类代码值：
                    "000"-"006" (ANSI 与 X/Open警告)
                    "I01"-"I11" (Informix指定)
                    "U01"       (用户自定义)

当有多个异常发生时，可根据GET DIAGNOSTICS返回的NUMBER值循环取出错误。
因为SQLSTATE是全局变量，只存储最近SQL语句发生的异常。所以要使用
RETURNED_SQLSTATE来取出某个错误的SQLSTATE代码值。


WHENEVER 语句让预编译器生成源代码，用以在发生错误、警告或缺少数据时评估 SQL 返
回码并转移到指定的标号。（如果未使用 WHENEVER 语句，默认行为是忽略 SQL 返回码)。

WHENEVER SQLERROR GOTO [Label]：指示预编译器生成源代码，用以在生成负的 sqlcode
值时评估 SQL 返回码并转移到指定标号。

WHENEVER SQLWARNING GOTO [Label]：指示预编译器生成源代码，用以在生成正的
sqlcode 值（除了值 100 之外）时评估 SQL 返回码并转移到指定标号。

WHENEVER NOT FOUND GOTO [Label]：指示预编译器生成源代码，用以在生成为 100 的
sqlcode 值或为 02000 的 sqlstate 值时评估 SQL 返回码并转移到指定标号。（100 值
用来表示没有找到与指定的选择条件匹配的记录，或者已经到达了结果数据集的末尾。）

WHENEVER [SQLERROR | SQL WARNING | NOT FOUND] CALL

WHENEVER [SQLERROR | SQL WARNING | NOT FOUND] CONTINUE：指示预编译器忽略 SQL

