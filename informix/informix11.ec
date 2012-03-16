/* 使用系统描述符区域探测 */
ALLOCATE DESCRIPTOR             分配系统描述符内存区域
DESCRIBE..USING SQL DESCRIPTOR  根据列列表信息初始化系统描述符区
GET DESCRIPTOR                  获得系统描述符区域字段的信息
SET DESCRIPTOR                  将信息放入系统描述符区域
/* 对于使用游标的SELECT and EXECUTE FUNCTION语句 */
OPEN..USING SQL DESCRIPTOR      从特定系统描述符区域获取输入参数
FETCH..USING SQL DESCRIPTOR     将行的内容放入系统描述符区域
/* 对于只返回单行的SELECT and EXECUTE FUNCTION语句 */
EXECUTE..INTO SQL DESCRIPTOR    将单行信息放入系统描述符区域
/* 对于非SELECT语句 */
EXECUTE..USING SQL DESCRIPTOR   从特定系统描述符区域获取输入参数
/* 对于使用插入游标的INSERT语句 */
PUT..USING SQL DESCRIPTOR       将行放入插入缓冲区，从SDA获取列值

DEALLOCATE DESCRIPTOR           释放分配的SDA


/* 分配SDA内存 */
默认分配描述符数目为100，可以加入WITH MAX子句扩大数目
初始化SDA中的COUNT字段
只是分配描述信息，而不是DATA字段


/* 初始化SDA */
设定COUNT字段，其值为SELECT、INSERT列列表中的列数目和表达式，或EXECUTE
FUNCTION返回的值数目
为每列初始化描述符字段
当使用SDA来获取输入参数时，不可使用DESCRIBE来初始化。必须用SET DESCRIPTOR显示
设定SDA中相应的字段

ALLOCATE DESCRIPTOR 为每一列描述符分配内存区域
DESCRIBE..USING SQL DESCRIPTOR 为每一描述符的DATA段分配内存。但是当

DESCRIBE..USING SQL DESCRIPTOR 对SELECT段从列中获取数据INTO lvarchar 宿主变量时并不分配内存。

当你fetch lvarchar数据到SDA时，必须显示指定列值的DATA段所用内存，如下：
1)，定义合适大小的lvarchar宿主变量
2)，用 SET DESCRIPTOR 指定宿主变量为DATA段。SET DESCRIPTOR 需要在
  DESCRIBE..USING SQL DESCRIPTOR 语句之后，但在 FETCH..USING SQL DESCRIPTOR语
  句之前。
3)，执行 FETCH..USING SQL DESCRIPTOR 语句以接受SDA中的列数据。

EXEC SQL BEGIN DECLARE SECTION;
lvarchar lvarch_val[50];
int i;
EXEC SQL END DECLARE SECTION;
EXEC SQL allocate descriptor ’desc’;
EXEC SQL prepare stmt1 from ’select opaque_col from table1’;
EXEC SQL describe stmt1 using sql descriptor ’desc’;
EXEC SQL declare cursor curs1 for stmt1;
EXEC SQL open curs1;
EXEC SQL set descriptor ’desc’ value 1
data = :lvarch_val, length = 50;

while (1) {
    EXEC SQL fetch curs1 using sql descriptor ’desc’;
    EXEC SQL get descriptor ’desc’ value 1 :lvarch_val;
    printf("Column value is %s\n", lvarch_val);

/* 从SDA中指定 与获取值 */
SET DESCRIPTOR 将值赋予SDA中的字段
GET DESCRIPTOR 从SDA字段中获取值

/* SET DESCRIPTOR */
设定COUNT字段匹配你要提供给SDA的描述符数量，通常是WHERE子句中输入参数的数目。
EXEC SQL set descriptor sysdesc COUNT=:hostvar;

为每列的值设定描述符字段。item_num为你想使用的列描述符的数目，DESCRIP_FIELD是
    描述符字段相应的值。
EXEC SQL set descriptor sysdesc VALUE :item_num
    DESCRIP_FIELD=:hostvar;
 set field值可为WHERE子句提供输入参数，或者修改SDA中相应描述符的内容。

TYPE字段的常量值在sqltypes.h中，但你不能直接在 SET DESCRIPTOR 语句中使用常量，
应当在 SET DESCRIPTOR 语句中将此常量值赋予一个宿主变量：

$int i;
i = SQLINT;
EXEC SQL set descriptor ’desc1’ VALUE 1 TYPE = :i;

/* 如果你在描述中使用 lvarchar指针宿主变量，并带有FETCH和PUT语句以使用SDA，你必
 你必须在SET DESCRIPTOR语句中显式指定TYPE类型为 124（CLVARCHARPTRTYPE）。*/

EXEC SQL BEGIN DECLARE SECTION;
lvarchar *lv;
EXEC SQL END DECLARE SECTION;
/* where tab has lvarchar * column */
EXEC SQL prepare stmt from “select col from tab”;
EXEC SQL allocate descriptor 'd';
/* The following describe will return SQLLVARCHAR for the
type of the column */
EXEC SQL describe stmt using sql descriptor 'd';
/* You must set type for *lv variable */
EXEC SQL set descriptor 'd' value 1 DATA = :lv, TYPE = 124;
EXEC SQL declare c cursor for stmt;
EXEC SQL open c;
EXEC SQL fetch c using sql descriptor 'd';

/* 使用GET DESCRIPTOR 语句*/
此句可以得到SDA中被描述的值的个数到COUNT字段

EXEC SQL get descriptor sysdesc VALUE :item_num
            :hostvar=DESCRIP_FIELD;
这些文件描述符值通常是被 SELECT/INSERT/EXECUTE FUNCTION说明。也常被用在
FETCH..USING SQL DESCRIPTOR 语句之后，用于拷贝数据库服务器返回的值到宿主变量之中。

/* 指定输入参数的值：*/
因为 DESCRIBE..USING SQL DESCRIPTOR 语句并不分析WHERE子句，你的程序必须将数
量/类型和输入参数的值存储在SDA区，用以显式描述这些参数。
当你执行一个参数化语句时，必须使用USING SQL DESCRIPTOR子句指定包含输入参数定位
的SDA区。
1，对于SELECT中的WHERE子句，使用 OPEN..USING SQL DESCRIPTOR. 这条语句控制一个
序列、滚动、UPDATE、HOLD游标。如果SELECT只是返回单行，使用 EXECUTE..INTO..USING
SQL DESCRIPTOR语句代替游标。
2，对于 DELETE/UPDATE 语句中的 WHERE 子句，使用EXECUTE..USING SQL DESCRIPTOR语
句。
3,对于 INSERT 语句中的 VALUES 子句，使用 EXECUTE..USING SQL DESCRIPTOR 语句。
如果INSERT与插入游标相关，则使用PUT..USING SQL DESCRIPTOR 语句替代。

/* 将数据库中的列值放入SDA */
当你动态创建一个SELECT语句，不能使用 FETCH..INTO hostvar子句，因为你不能在
prepared语句中使用宿主变量。需要使用 FETCH..USING SQL DESCRIPTOR 语句将每一列
的值放入它们表示符的DATA段。
对于多值，必须使用SELELCT游标和函数游标。
如果单行，使用 EXECUTE..INTO SQL DESCRIPTOR

一旦列值放入了SDA，可使用 GET DESCRIPTOR 从他们的数据字段中放入合适的主机变量
。必须设置 LENGTH 和 TYPE 噢。

/* 此时参考demo4.ec例子程序 */


/* 执行一个游标函数 */
1，为自定义函数定义一个函数游标。
DECLARE ....CURSOR EXECUTE FUNCTION
2，OPEN 语句执行函数并打开游标
3,FETCH..USING SQL DESCRIPTOR 语句，接受从游标返回的值，放入SDA区
4,GET DESCRIPTOR 语句，接受SDA区中返回的值，放入SDA区
5,DEALLOCATE DESCRIPTOR

SPL函数如果在RETURN语句带有 WITH RESUME 关键字可以返回多行值。

/* 操作未知列列表基本步骤 */

/* 处理INSERT中的输入参数 */
1,PREPARE INSERT 语句，定义一个表示符
2,ALLOCATE DESCRIPTOR 语句
3,探测列的数量和数据类型。使用 DESCRIBE..USING SQL DESCRIPTOR 语句。DESCRIBE
语句将每一列的描述符放入选择列表。
4,GET DESCRIPTOR 语句将未知列的数目COUNT存入宿主变量。
5,SET DESCRIPTOR 语句将设定相关的DATA和VALUE字段。如果想插入NULL值，设定
INDICATOR 字段为-1,并且不在 SET DESCRIPTOR 语句设定任何DATA段。
6,设定INSERT语句以在数据库插入值。
7，DEALLOCATE DESCRIPTOR

/* 处理INSERT游标 */
1,PREPARE INSERT 语句，之后 DECLARE..CURSOR..
2,使用 OPEN 打开（创建）INSERT游标
3,PUT..USING SQL DESCRIPTOR。PUT 之后，列值从SDA放入INSERT缓冲区。重复如此直到
没有行。
4,FLUSH
5,CLOSE。cursorname

/* 处理参数化的SELECT语句 */
/* 使用SDA定义WHERE子句中的输入参数 */
1,探测输入参数的数量和数据类型
2,ALLOCATE DESCRIPTOR 
3,WHERE..SET DESCRIPTOR COUNT..指定WHERE子句中输入参数的数量
4,对每一子句都使用 SET DESCRIPTOR 设定 DATA/TYPE/LENGTH 字段。
注意：CHAR VCHAR类型，LENGTH为字符数组的字节大小。DATETIME INTERVAL类型的
LENGTH为编码限定大小。
如果使用了指示变量，还要设定IDATE ILENGTH ITYPE字段。使用VALUE 关键字识别每个描述符。
从SDA获取输入参数的定义，使用 USING SQL DESCRIPTOR 子句

/* 执行参数化的返回多行的SELECT语句 */
OPEN 语句打开游标
FETCH..USING SQL DESCRIPTOR 语句获取返回值
如含有 WHERE 子句中的输入参数，则 OPEN 语句包含 USING SQL DESCRIPTOR 子句。
如含有未知列，FETCH ..USING SQL DESCRIPTOR 语句会将返回值写入SDA


/* 执行参数化的单例SELECT */
使用 EXECUTE..USING SQL DESCRIPTOR..INTO 语句 代替游标SELECT中的 OPEN..USING
SQL DESCRIPTOR


