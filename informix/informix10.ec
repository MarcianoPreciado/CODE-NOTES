/* 动态管理数据结构 */
1，system-descriptor area，系统描述符区域，是遵循X/Open标准的数据结构，
ALLOCATE DESCRIPTOR, GET DESCRIPTOR, SET DESCRIPTOR, and DEALLOCATE DESCRIPTOR.

2，sqlda是用C语言构建的数据结构，与X/Open标准不同。


/* 系统描述符区域 */
系统描述符区域是ESQL/C为控制返回、发送数据而分配的一块内存区域。
有两个部份：

1)，定长部份：COUNT字段，包括系统描述符区域列的数目。
由 DESCRIBE...USING SQL DESCRIPTOR 语句设定COUNT。在发送列值到数据库服务器前，
必须使用 SET DESCRIPTOR 语句来初始化此字段。

2)，变长部份：包括系统描述符区域每个值的定长描述符。
每个描述符包含以下内容:

Field       DATA TYPE       Description
***************************************************************************
DATA        char *          列数据的指针
TYPE        short           表示列数据的类型，在sqltypes.h和sqlxtype.h中定义
LENGTH      short           类型的长度，单位为字节
NAME        char *          列名或正被传输的label
INDICATOR   short           值为0，表示DATA字段包括非NULL数据
                            值为-1，表示没有指定数据值是插入NULL
SCALE       short           数据段中列数据的等级，仅被DECIMAL和MONEY类型定义
PRECISION   short           列数据的精度，仅被DECIMAL和MONEY类型定义
NULLABLE    short           列是否可以包含NULL1，允许，0，不允许
IDATA       char *          用户自定义指示数据或指示变量数据段
ITYPE       short           用户自定义指示变量类型
ILENGTH     short           用户自定义指示变量长度
EXTYPEID    int4            用户自定义(opaque或distinct）或复合(集合或行)类型
                                       数据的扩展标识符。
EXTYPEID    int4            扩展数据名字
ETYPELENGTH short           扩展名字段的长度，单位为字节
EXTYPEOWNERNAME     char*   用户自定义和符合数据类型拥有者的名字
EXTYPEOWNERLENGTH   short   名字的长度
SOURCEID    int4            distinct-type 列源数据的扩展类型。


/* sqlda数据结构 */
sqlda结构有3部份：

1)，定长部份：sqlda结构中列字段的数目，由 DESCRIBE...INTO 语句设定sqld字段。

2)，变长部份：每个值的sqlvar_struct数据结构，sqlvar_struct为定长数据结构。
sqlda结构中的sqlvar字段指向sqlvar_struct结构数组的起始地址。每一个
sqlvar_struct结构对应一个数据信息。 
/* struct sqlvar_struct结构 */

Filed       Data Type       Description
****************************************************************************
sqltype     short           列数据的类型
sqllen      short
sqldata     char *          列数据的指针
sqlind      short *         列数据指示变量的指针。值为0，有非NULL。值为-1，NULL
sqlname     char *          列数据的名字或者label名
sqlformat   char *          保留字段
sqlitype    short           自定义指示变量数据类型
sqlidata    char *          指向用户自定义指示变量数据的指针
sqlxid      int4            扩展标识符
sqltypename char *          扩展类型名字,用户自定义(opaque或distinct）或复合(
                             集合或行)类型名字
sqltypelen  short           扩展类型名字长度。
sqlownerlen short           扩展类型拥有者名字
sqlsourcetype short         内建的distinct源数据类型
sqlsourceid int4            扩展类型中的源类型
sqlflags    int4            



3)，sqlda自己的描述信息。

Filed       Data Type       Description
****************************************************************************
desc_name   char[19]        描述符名字
desc_occ    short           sqlda数据结构大小
desc_next   struct sqlda *  下一个sqlda结构

DESCRIBE 语句可以在以下数据结构中写信息
1，DESCRIBE...USING SQL DESCRIPTOR 在系统描述符区域存储信息。可以通过 GET
DESCRIBE 语句获取这些信息。
2，DESCRIBE...INTO sqlda_ptr 在sqlda_ptr指针所指向的sqlda结构中存储信息。


如果DESCRIBE成功：
SQLCODE值指示准备好的语句类型。
sqlstype.h头文件中定义了SQL语句类型的常量。检查SQLCODE对应的值确定语句。
如果DESCRIBE语句描述了DELETE或UPDATE，可以指示语句段是否有WHERE子句。

sqltypes.h头文件包含Informix ESQL/C指定的SQL数据类型常量，默认为DESCRIBE使用。
sqlxtype.h定义了X/Open SQL数据类型常量。DESCRIBE在你编译.ec文件使用了-xopen预
处理选项时使用。

/* 探测输入参数 */
你可以使用 DESCRIBE 和 DESCRIBE INPUT 为一个已经准备好但还没有执行的语句返回输
入参数信息。
DESCRIBE INPUT 语句返回值的数量，数据类型，大小，列的名字或者查询表达式。可返回:

带有 IN、BETWEEN、HAVING、ON 子句的 SELECT 语句
SELECT 子查询
SELECT INTO TEMP
DELECT
EXECUTE

/* 带有 WHERE 子句的 INSERT\UPDATE 语句 */
/* 在DESCRIBE分析一个预备好的DELETE和UPDATE语句时，如果包含WHERE子句，设定
 * sqlca.sqlwarn.sqlwarn0 和 sqlca.sqlwarn.sqlwarn4 字段为W。设定SQLSTATE值为
 * Informix指定警告值，"01I07"。
 */

/* 操作未知语句信息 */
1)，定义动态管理结构获取信息。可以是系统描述符区域或者SQLDA结构。
2)，使用DESCRIBE语句检查准备好的SELECT语句和描述的各列。
3)，指定动态管理结构定位从数据库获取的数据。

/* 使用Fetch数组 */
单一 FETCH 语句从fetch缓冲区到sqlda结构的行数。可由fetch数组增加。
当你fetch simple-large-object时特别有用。

如果不使用fetch数组获取simple large object:
当ESQL/C fetch一个TEXT或BYTE列时，服务器返回列的描述符。
ESQL/C随后请求服务器发送相应的数据。

如果使用fetch数组：
ESQL/C发送一系列的simple-large-object描述符给数据库服务器
数据库服务器在同一时间发送所有的数据。


