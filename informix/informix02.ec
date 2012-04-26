/* Informix数据类型常量，均定义在sqltypes.h文件中。
 * 包括SQL数据类型常量。宿主C语言数据类型，例如
 * double CDOUBLETYPE 105
 * dec_t or decimal CDECIMALTYPE 107
 * fixchar CFIXCHARTYPE 108
 * string CSTRINGTYPE 109
 * date CDATETYPE 110
 * dec_t or decimal CMONEYTYPE 111
 * datetime or dtime_t CDTIMETYPE 112
 * ifx_loc_t or loc_t CLOCATORTYPE 113
*/

/*
当两个数据的类型不一致时，Informix会自动转换他们的类型。
1，比较。例如 CHAR 邮编与 INT整型比对。
2，Fetch or insert
3，算数运算。
*/

/* C TO N或 N TO C空间不足时，sqlca.sqlwarn.sqlwarn1值为W，且SQLSTATE值为
 * 01004。
 * BOOLEAN TO C时，BOOLEAN值'\01','\00'自动转换为FIXCHAR值 'T'或'F'。
 *
 * 算术转换：
 * Operand Type   Convert To
 * FLOAT          DECIMAL(17)
 * SMALLFLOAT     DECIMAL(9)
 * INTEGER        DECIMAL(10,0)
 * SMALLINT       DECIMAL(5,0)

 For example, for an arithmetic operation between a fixed-point decimal of
 DECIMAL(8,3) and a FLOAT value, Informix ESQL/C converts the FLOAT value to a
 floating-point decimal of DECIMAL(17). The arithmetic result has a data type
 of DECIMAL(17).

 Function Name      Description See
The risnull()       Checks whether a C variable is null B-191
The rsetnull()      Sets a C variable to null B-197
The rtypalign()     Aligns data on correct type boundaries B-206
The rtypmsize()     Gives the byte size of SQL data types B-209
The rtypname()      Returns the name of a specified SQL data type B-211
The rtypwidth()     Returns the minimum number of characters that a character
data type needs to avoid truncation */


/*
 * This section describes the characteristics of these data types and the differences
 *among them.

 * Table 4-1. ESQL/C Character Data Types
 * ESQL/C Character Data Type   Null Terminated     Contains Trailing Blanks
 * char                         Y                   Y
 * fixchar                      N                   Y
 * string                       Y                   Returns a trailing blank
                                                    only if the column contains an empty string.
 * varchar                      Y                   Y
 * lvarchar                     Y
 */

你可以将带有\0结束符的C字符串放入FIXCHAR变量（空间要够大）。但这不是一个好主意
，当数据库服务器将这个FIXCHAR放入列中时，终结符也被放入。
/*
 * When the IFX_PAD_VARCHAR environment variable is set to 1, the client sends
 * the VARCHAR data type with padded trailing spaces. When this environment is
 * not set (the default), the client sends the VARCHAR data type value without
 * trailing spaces. The IFX_PAD_VARCHAR environment variable must be set only at
 * the client side and is supported only with IBM Informix ESQL/C Version 9.53 and
 * 2.90 or later and IBM Informix Version 9.40 or later.
 *
 */
如果你不指定lvarchar宿主变量的大小，它等同于一字节的char类型。
如果指定size，等同于C语言同等大小的CHAR类型。
在定义SQL类型LVARCHAR到ESQL/C类型lvarchar或char时，size值要加1,以存放终结字符。
如果lvarchar宿主变量是一个指针，指针指向的数据最大可以2GB。
lvarchar类型设计为当你insert和select用户自定义或不明类型时，可以表现为字符串格
式。必须使用 ifx_var() 函数操作lvarchar指针。

/* An application can fetch data from a database column of type CHAR or
 * VARCHAR into a character (char, string, fixchar, varchar, or lvarchar) host
 * variable. If the column data does not fit into the character host variable,
 * Informix ESQL/C truncates the data. To notify the user of the truncation,
 * Informix ESQL/C performs the following actions: It sets the
 * sqlca.sqlwarn.sqlwarn1 warning flag to W and the SQLSTATE variable to
 * 01004.  It sets any indicator variable that is associated with the
 * character host variable to the size of the character data in the column.
 *
 *
 * Inserting CHAR Data An application can insert data from a character host
 * variable (char, string, fixchar, varchar, or lvarchar) into a database
 * column of type CHAR. If the value is shorter than the size of the database
 * column then the database server pads the value with blanks up to the size
 * of the column.  If the value is longer than the size of the column the
 * database server truncates the value if the database is non-ANSI. No warning
 * is generated when this truncation occurs. If the database is ANSI and the
 * value is longer than the column size then the insert fails and this error
 * is returned: -1279: Value exceeds string column length.
 *
 */

/* 以by函数开头的函数操作和返回fixed-length字符串。
 * 以rst和st（除了stchar)开头的函数，操作均返回终结符结尾的字符串。
 * rdownshit和rupshit操作有终结符的字符串，但是不返回值。
 */

Function Name   Description
bycmpr()        Compares two groups of contiguous bytes B-10
bycopy()        Copies bytes from one area to another B-12
byfill()        Fills an area you specify with a character B-13
byleng()        Counts the number of bytes in a string B-14 不统计结尾空白符
ldchar()        Copies a fixed-length string to a null-terminated string B-170
rdownshift()    Converts all letters to lowercase B-176
rstod()         Converts a string to a double value B-200
rstoi()         Converts a string to a short integer value B-201
rstol()         Converts a string to a 4-byte integer value B-202
rupshift()      Converts all letters to uppercase B-209
stcat()         Concatenates one string to another B-232
stchar()        Copies a null-terminated string to a fixed-length string 填充空白
stcmpr()        Compares two strings B-234
stcopy()        Copies one string to another string B-235
stleng()        Counts the number of bytes in a string B-235

