/* 数据库系统安全主管 (DBSSO)
    控制审计子系统监视的对象以及数据库服务器可执行的操作
审计分析主管 (AAO)
    控制审计是否发生、维护审计日志文件和分析
    DBSSO要求审计的那些数据库活动a的审计记录。

必须选择定制安装设置才能启用角色分离。


安装时未选择“允许安装程序保护路径的安全"，在安装之后的解决方法：
运行以下命令来生成 secure.sh 脚本：$INFORMIXIDIR/bin/onsecurity -r $INFORMIXDIR
运行 secure.sh 脚本来保护安装路径的安全：$INFORMIXDIR/tmp/secure.sh

 较新版本jre可能与ids不兼容，可以选择-javahome none来使用ids自带的jre。
sudo ./ids_install -javahome none */




/* /etc/profile.d/informix.sh中的变量设置：*/

INFORMIXDIR=/home/informix/informix
INFORMIXSERVER=ol_informix
ONCONFIG=onconfig.ol_informix
INFORMIXSQLHOSTS=/home/informix/informix/etc/sqlhosts.ol_informix
PATH=${INFORMIXDIR}/bin:${INFORMIXDIR}/extend/krakatoa/jre/bin:${PATH}
export INFORMIXDIR INFORMIXSERVER ONCONFIG INFORMIXSQLHOSTS PATH

DB_LOCALE=zh_cn.utf8
SERVER_LOCALE=zh_cn.utf8
CLIENT_LOCALE=zh_cn.utf8
GL_USEGLU=1
export DB_LOCALE SERVER_LOCALE CLIENT_LOCALE GL_USEGLU

GL_DATE="%iy/%m/%d"
DATETIME="%iY-%m-%d %H:%M:%S"
export GL_DATE DATETIME

/*
根据$INFORMIXDIR/etc/sqlhosts来修改/etc/services文件和/etc/hosts文件。

准备 Informix 配置文件：（本人是直接使用的安装过程中选择的demon_on服务器实例。

    复制 onconfig.std 模板文件。
    修改模板文件的副本。 DUMPDIR 参数的缺省值是 $INFORMIXDIR/tmp。如果在配置文件中更改此值，请确保指定计算机上的有效目录。
    将 ONCONFIG 环境变量设置为定制配置文件的名称。

要点： 只有用户 informix 或 root 用户才能初始化数据库服务器。
数据库服务器至少必须进行一次初始化。 oninit -i
覆盖现有数据库。不带-i选项则不覆盖。
*/

/* 因为esql与C都将'\'作为转义字符，且依次进过esql、c处理。所以如要查找字符串 "\"
 * （3字节）的话，应当使用 */
EXEC SQL SELECT col1 FROM tab1 WHERE col1 = ’\"\\\\\"’;

/* 默认不允许在字符串类型中加入换行符
 * 方法1：在ONCONFIG文件中配置ALLOW_NEWLINE
 * 方法2：如下： */
EXEC SQL EXECUTE procedure ifx_allow_newline(’t’);

EXEC SQL BEGIN DECLARE SECTION;
-- put host variable declarations here
EXEC SQL END DECLARE SECTION;

/*　当 DELIMIDENT 设置 YES 时，双引号字符串识别 SQL 标识符。当 DELIMIDENT
 * 没有设置 YES 时，双引号字符串被当作字符串字面值。
 *
 * 在ESQL/C中，变量类型，宿主变量区分大小写。单SQL关键字、语句表示符和游标名
 * 不区分大小写。
 *
 * 定义宿主变量作为参数。
 * 宿主变量是你在嵌入式SQL中使用的ESQL/C和C变量。SELECT 。。INTO :hostvar1,
 * :hostvar2，如果省略掉hostvar1和hostvar2之间的逗号，hostvar2就作为hostvar1的指
 * 示变量。
 *
 * ANSI C风格参数类型声明如下： 注意此时因为在行参括号之内，所以用逗号。
*/
int * foo(
EXEC SQL BEGIN DECLARE SECTION;
PARAMETER char s[20],
PARAMETER int id,
PARAMETER int (*f) (double)
EXEC SQL END DECLARE SECTION;
)
{
select fname into :s from customer
where customer_num = :id;

/* 老C K&R风格 */
f(s, id, s_size)
EXEC SQL BEGIN DECLARE SECTION;
PARAMETER char s[20];
PARAMETER int id;
EXEC SQL END DECLARE SECTION;
int s_size;
{
select fname into :s from customer
where customer_num = :id;

/* 指示变量不能使用DATETIME型或INTERVAL型。*/
:hostvar INDICATOR :indvar
:hostvar :indvar
$hostvar $indvar
/* 如果ESQL/C语句对宿主变量返回NULL值，指示变量值设为 -1。
 *
 * 如果指示变量值并没有事先定义，使用esql -icheck选项时，ESQL会设置
 * sqlca.sqlcode值。
 *
 * 如果宿主变量使用成功，指示变量设为0。
 * 如果值被截短，则指示变量值设为SQL列值的实际字节大小，且设置SQLSTATE
 * 为"01004"。同时设置SQLWARN1为W
 */
/* Make sure to terminate the line of code with a semicolon. Some additional
examples follow: ESQL/C头文件不必以.h结尾。*/
EXEC SQL include varchar.h;
EXEC SQL include sqlda;
$include sqlstype;
EXEC SQL include "C:\apps\finances\credits.h"; /*fullpath头文件用双引号括起来 */

