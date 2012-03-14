ESQL/C
集合表类型:LIST, MULTISET, SET
行类型:命名的或未命名的

SET集合中的元素值均为唯一，且无序。
MULTISET允许重复值。
LIST集合中的元素值可重复，有序的。

create row type person(name char(255), id int);
create table parents(name char(255), id int, children list(person not null));

/* 从parent表中获取children's names and id:*/
select name, id from table(select children from parents
                           where parents.id = 1001) c_table(name, id);
执行此查询时，创建了一个满足 p.id = 1001 的虚拟表 c_table。

将集合作为虚拟表来查询更加高效，不必使用很多宿主变量和游标。

EXEC SQL create row type parent_type(name char(255), id int,
children list(person not null));
EXEC SQL create grade12_parents(class_id int,
parents set(parent_type not null));

EXEC SQL select name into :host_var1
from table((select children from table((select parents
from grade12_parents where class_id = 1))
p_table where p_table.id = 1001)) c_table
where c_table.name like 'Mer%';

/* 不使用集合的例子 */
EXEC SQL client collection hv1;
EXEC SQL client collection hv2;
EXEC SQL int parent_id;
EXEC SQL char host_var1[256];
.
.
.
EXEC SQL allocate collection hv1;
EXEC SQL allocate collection hv2;
EXEC SQL select parents into :hv1 from grade12_parents
        where class_id = 1;
EXEC SQL declare cur1 cursor for select id, children
        from table(:hv1);
EXEC SQL open cur1;
for(;;) {
    EXEC SQL fetch cur1 into :parent_id, :hv2;
    if(parent_id = 1001)
        break;
}
EXEC SQL declare cur2 cursor for select name from
        table(:hv2));
EXEC SQL open cur2;
for(;;) {
    EXEC SQL fetch cur2 into :host_var1;
/* user needs to implement 'like' function */
    if(like_function(host_var1, "Mer%"))
        break;
}

/* 使用集合表类型的限制 */
1，不可以是INSERT， DELETE, UPDATE语句的目标。
2，不可以作为可更新的游标或视图以下的表。
3，不支持序列ordinality。
4，不可以重复引用在统一个FROM子句中的表。例如：
select count(distinct c_id) from parents,
       table(parents.children) c_table(c_name, c_id)
        where parents.id = 1001;
5,数据库服务器必须静态指定下集合表达式的数据类型。
6，The database server cannot support a reference to a host variable without
casting it to a known collection type. For example, rather than specifying
TABLE(:hostvar), you must cast the host variable:
TABLE(CAST(:hostvar AS type))
TABLE(CAST(? AS type))
7，集合表类型LIST不可以保持行的顺序。
8，集合中的数据类型不可以是SERIAL，SERIAL8，BIGSERIAL

DECLARE集合类型变量的时候，必须显式指定CLIENT关键字。
1，显示指定collection变量：指定集合中元素的类型和集合自身类型。
EXEC SQL BEGIN DECLARE SECTION;
    CLIENT COLLECTION LIST(SMALLINT NOT NULL) LIST1;
    CLIENT COLLECTION SET(ROW(x CHAR(20),
                              y SET(INTEGER NOT NULL),
                              z DECIMAL(10,2)) NOT NULL) row_set;
    CLIENT COLLECTION MULTISET(SET(SMALLINT NOT NULL)
                               NOT NULL) collection3;
EXEC SQL END DECLARE SECTION;

集合类型中的元素类型必须是内建类型。BYTE，TEXT，SERIAL，SERIAL8 都不可以。
且必须对元素类行进行NOT NULL限制。

(1)，命名的row类型不可以作为元素类型。然后，你可以指定与已命名row类型字段相同的元素类型。

CREATE ROW TYPE myrow
(
    a int,
    b float
);
CREATE TABLE mytable
(
    col1 int8,
    col2 set(myrow not null)
);
/* 未命名ROWL类型 */

EXEC SQL BEGIN DECLARE SECTION;
client collection set(row(a int, b float) not null)
my_collection;
EXEC SQL END DECLARE SECTION;

(2)，你可以显式定义一个与数据库集合列元素类型相同的集合，只要他们的元素类型可以互相兼容。


EXEC SQL BEGIN DECLARE SECTION;
    client collection set(float not null) set_float;
    client collection set(integer not null) set_int;
EXEC SQL END DECLARE SECTION;

EXEC SQL declare cur1 cursor for select * from tab1;
open cur1;
fetch cur1 into:set_float;
fetch cur1 into :set_int; /* 第一次fetch后，定义了元素类型为float，所以此时产
                             生类型不匹配错误。*/





2，隐式指定：元素类型和集合自身类型均不指定。
仅包含COLLECTION关键字和集合变量名字。
如果你不知道你想access的集合元素的精确类型，就使用隐式collection变量。
你必须获取集合列的定义情况，以便使用未命名collection变量。

EXEC SQL BEGIN DECLARE SECTION;
client collection a_coll;


当SQL语句包含一个clollection变量，它有以下语法限制：
1，只可以通过在客户端使用集合衍生表 TABLE(:COLLEC)子句，与 SELECT INSERT
UPDATE DELETE 操作来获取元素。
2，INSERT 语句不能在VALUES子句中包含 SELECT，EXECUTE FUNCTION，或者 EXECUTE
PROCEDURE。
3，不能包含 WHERE 子句或表达式
4，不可以使用游标


ESQL/C并不自动分配或释放集合变量的内存，你必须显式指定。
当指向集合的游标处于OPEN状态时，DEALLOCATE COLLECTION 此集合失败

集合列与集合变量的区别：
当你使用集合衍生表对集合变量进行操作时，语句并不发送给服务器进行处理，而是
ESQL/C处理。
当你对集合衍生表进行UPDATE或INSERT操作时，必须带有SET或VALUES子句。

可以使用SELECT 集合列 INTO 集合变量 FROM 表名 的方式来对隐式集合变量进行初始化
。并且可以 SELECT 任何类型的集合列 INTO 隐式宿主集合变量。


EXEC SQL ALLOCATE COLLECTION :time_vals;
EXEC SQL SELECT TIME_DATASET INTO :time_vals
FROM readings
WHERE dataset_id = 1356;
IFX_INT8CVINT(1423231, &an_int8);
EXEC SQL INSERT INTO TABLE(:time_vals) VALUES (:an_int8);

/* 插入游标 */
插入LIST集合类型时，可以使用 INSERT AT 3 INTO TABLE(:hostcollec)。。。。
插入游标允许你向集合变量插入多个值。但必须是顺序游标，不能是滚动游标和WITH HOLD游标。

EXEC SQL PREPARE ins_stmt FROM
        ’INSERT INTO TABLE VALUES’;
EXEC SQL DECLARE list_curs CURSOR FOR ins_stmt;
EXEC SQL OPEN list_curs USING :a_list;

while (1) {
    EXEC SQL PUT list_curs FROM :an_element;
..........
}
EXEC SQL CLOSE list_curs;
EXEC SQL INSERT INTO tab_list(:alist);
EXEC SQL DEALLOCATE :alist;
EXEC SQL FREE ins_stmt;
EXEC SQL FREE list_curs;

/* SELECT游标 */

