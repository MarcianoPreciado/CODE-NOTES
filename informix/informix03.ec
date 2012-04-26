
/* 如果想用字符('F','T')对SQL BOOLEAN列赋值，则必须先定义FIXCHAR并赋其值。*/

EXEC SQL BEGIN DECLARE SECTION;
boolean flag;
fixchar my_boolflag;
int id;
EXEC SQL END DECLARE SECTION;

id = 1;

flag = ’\0’; /* valid boolean assignment to FALSE */
EXEC SQL insert into table2 values (:id, :flag); /* inserts FALSE */

my_boolflag = ’T’ /* valid character assignment to TRUE */
EXEC SQL insert into table2 values (:id, :my_boolflag); /* inserts TRUE*/

rsetnull(CBOOLTYPE, (char *) &flag); /* valid BOOLEAN assignment to NULL */

EXEC SQL include decimal;
/* DECIMAL(p)浮点和DECIMAL(p,n)定点。后者是ANSI SQL标准。*/

#define DECSIZE 16
#define DECUNKNOWN -2

struct decimal
    {
    int2 dec_exp;		/* exponent base 100		*/
    int2 dec_pos;		/* sign: 1=正数, 0=负数, -1=null	*/
    int2 dec_ndgts;		/* number of significant digits	*/
    char  dec_dgts[DECSIZE];	/* actual digits base 100	*/
    };
typedef struct decimal dec_t;
/*  存储最多32个有效位的浮点数。注意是BASE 100。每个dec_dgts[x]存储两位数。*/

/* Function     Description */
decadd()        Adds two decimal numbers B-15
deccmp()        Compares two decimal numbers B-17
deccopy()       Copies a decimal number B-18
decdiv()        Divides two decimal numbers B-27
decmul()        Multiplies two decimal numbers B-32
decround()      Rounds a decimal number B-34
decsub()        Subtracts two decimal numbers B-35
dectrunc()      Truncates a decimal number B-43
deccvasc()      Converts a C char type value to a decimal type value
deccvdbl()      Converts a C double type value to a decimal type value
deccvint()      Converts a C int type value to a decimal type value
deccvlong()     Converts a C 4-byte integer type value to a decimal type value
dececvt()       Converts a decimal value to an ASCII string 类似于ecvt(3)
decfcvt()       Converts a decimal value to an ASCII string fcvt(5)
dectoasc()      Converts a decimal type value to an ASCII string
dectodbl()      Converts a decimal type value to a C double type value
dectoint()      Converts a decimal type value to a C int type value
dectolong()     Converts a decimal type value to a C 4-byte integer type value

/* 数字格式化显示 */
*       显示*号代替空格
&       用0填充空格占位
#       代表一个数字或空格的位置。
<       左调整。
,       默认显示,仅在左边有数字时显示，可以设置DBMONEY变量来更改显示字符。
.       一个数字模板只能显示一个. ，可设置DBMONEY环境变量更改显示。
-       数字是负的时候显示。 ---,-##.## 对应的-12.34 显示 bbbb-12.34。
+       显示正号。
(
) 同上配对显示括号的负值。
$ 美元符号，可设值DBMONEY环境变量更改。

DBMONEY环境变量默认为"$,."。

rfmtdec()       Converts a decimal value to a string B-181
rfmtdouble()    Converts a double value to a string B-183
rfmtlong()      Converts a 4-byte integer value to a string B-185

/* 日期格式化显示 */
ddd                 Sun through Sat
mmm                 Jan through Dec

rdatestr()			 Converts an internal DATE to a character string format B-171
rdayofweek()		 Returns the day of the week of a date in internal format B-172
rdefmtdate()		 Converts a specified string format to an internal DATE B-174
rfmtdate()			 Converts an internal DATE to a specified string format B-178
rjulmdy()			 Returns month, day, and year from a specified DATE B-193
rleapyear()			 Determines whether specified year is a leap year B-195
rmdyjul()			 Returns an internal DATE from month, day, and year B-196
rstrdate()			 Converts a character string format to an internal DATE B-204
rtoday()			 Returns a system date as an internal DATE B-205

EXEC SQL BEGIN DECLARE SECTION;
DATETIME YEAR TO DAY sale;
INTVAL HOUR TO SECOND test_num;
EXEC SQL END DECLARE SECTION;

/* 可以使用DBDATE和GL_DATE环境变量来定制服务器期望的DATE格式。
 * 可以使用DBTIME和GL_DATETIME环境变量来定制服务器所期望的DATETIME格式。
*/
在头文件datetime.h中有定义了八个名字和五个宏,可以在操作DATETIME和INTERVAL
值时使用它们:
TU_YEAR         用于year修饰符域的名字
TU_MONTH        用于month修饰符域的名字
TU_HOUR         用于hour修饰符域的名字
TU_DAY          用于day修饰符域的名字
TU_MINUTE       用于minute修饰符域的名字
TU_SECOND       用于second修饰符域的名字
TU_FRAC         用于开始的fraction修饰符域的名字
TU_Fn           用于结束的fraction(n)修饰符域的名字(n=1-5)
TU_LEN(q)       返回修饰符q中的数字的位数。
TU_START(q)     从修饰符q返回开始单元。
TU_END(q)       从修饰符q返回结束单元。
TU_DTENCODE(s,e)    从开始单元s和结束单元e组合成DATETIME修饰符。
TU_IENCODE(p,s,e)   从带有精度p的开始单元s和结束单元e组合成INTERVAL修饰符。


当把DATETIME和INTERVAL数据库值存放到datetime和interval宿主变量时，在宿主变量的
定义时可以省略修饰符。同时，也不必初始化修饰符。在这种情形下，数据库服务器把
dt_qual或in_qual域初始化为与数据库中的定义同样的值。返回值是数据库中的值。

可以让数据库服务器自动把DATETIME和INTERVAL类型的数据库值转换成字符串。在把这些
值放到宿主变量：char、string或fixchar之前，会进行隐式的数据转换。

To explicitly convert a DATETIME value to a DATE value:
1. Use the dtextend() function to adjust the DATETIME qualifier to year to day.
2. Apply the dttoasc() function to create a character string in the form yyyy-mm-dd.
3. Use The rdefmtdate() function with a pattern argument of yyyy-mm-dd to
convert the string to a DATE value.



To explicitly convert a DATE value to a DATETIME value:
1. Declare a host variable with a qualifier of year to day (or initialize the qualifier
with the value that the TU_DTENCODE(TU_YEAR,TU_DAY) macro returns).
2. Use the rfmtdate() function with a pattern of yyyy-mm-dd to convert the DATE
value to a character string.
3. Use the dtcvasc() function to convert the character string to a value in the
prepared DATETIME variable.
4. If necessary, use the dtextend() function to adjust the DATETIME qualifier.

INTERVAL值遵循一下两组限定：
1，YEAR TO MONTH
2，DAY TO FRACTION
或者是两组集合中的子集。

Function        Name Description
dtaddinv()      Adds an interval value to a datetime value B-45
dtcurrent()     Gets the current date and time B-46
dtcvasc()       Converts an ANSI-compliant character string to a datetime value
dtcvfmtasc()    Converts a character string with a specified format to a datetime value
dtextend()      Changes the qualifier of a datetime value B-52
dtsub()         Subtracts one datetime value from another B-53
dtsubinv()      Subtracts an interval value from a datetime value B-55
dttoasc()       Converts a datetime value to an ANSI-compliant character string
dttofmtasc()    Converts a datetime value to a character string with a specified format
incvasc()       Converts an ANSI-compliant character string to an interval value
incvfmtasc()    Converts a character string with a specified format to an interval value
intoasc()       Converts an interval value to an ANSI-compliant character string
intofmtasc()    Converts an interval value to a character string with a specified format
invdivdbl()     Divides an interval value by a numeric value B-161
invdivinv()     Divides an interval value by another interval value B-163
invextend()     Extends an interval value to a different interval qualifier
invmuldbl()     Multiplies an interval value by a numeric value B-167

