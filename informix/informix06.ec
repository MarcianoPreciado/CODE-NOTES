ESQL/C
<<<<<<< HEAD
集合类型:LIST, MULTISET, SET
行类型:命名的或未命名的

SET类型存储 元素值均为唯一，且无序的集合。
MULTISET类型存储允许重复的元素值，且无序的集合。
LIST类型存储元素值可重复，且有序的集合。

SQL允许你对集合执行一个只读的SELECT操作并实现一个集合衍生表作为虚拟表。

对集合进行SELECT操作时，并不引用一个ESQL/C集合宿主类型。
应如下操作：
=======
集合表类型:LIST, MULTISET, SET
行类型:命名的或未命名的

SET集合中的元素值均为唯一，且无序。
MULTISET允许重复值。
LIST集合中的元素值可重复，有序的。
>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c

create row type person(name char(255), id int);
create table parents(name char(255), id int, children list(person not null));

/* 从parent表中获取children's names and id:*/
select name, id from table(select children from parents
                           where parents.id = 1001) c_table(name, id);
执行此查询时，创建了一个满足 p.id = 1001 的虚拟表 c_table。

将集合作为虚拟表来查询更加高效，不必使用很多宿主变量和游标。

EXEC SQL create row type parent_type(name char(255), id int,
<<<<<<< HEAD
                                    children list(person not null));
EXEC SQL create grade12_parents(class_id int,
                                parents set(parent_type not null));

EXEC SQL select name into :host_var1
                from table((select children from table((select parents
                                from grade12_parents where class_id = 1)) p_table
                            where p_table.id = 1001)) c_table
                where c_table.name like 'Mer%';
=======
children list(person not null));
EXEC SQL create grade12_parents(class_id int,
parents set(parent_type not null));

EXEC SQL select name into :host_var1
from table((select children from table((select parents
from grade12_parents where class_id = 1))
p_table where p_table.id = 1001)) c_table
where c_table.name like 'Mer%';
>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c

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
<<<<<<< HEAD
                                    where class_id = 1;
EXEC SQL declare cur1 cursor for select id, children
                            from table(:hv1);
=======
        where class_id = 1;
EXEC SQL declare cur1 cursor for select id, children
        from table(:hv1);
>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c
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
<<<<<<< HEAD
1，不可以是 INSERT， DELETE, UPDATE 语句的目标。
2，不可以作为可更新的游标或视图以下的表。
3，不支持序列ordinality。
4，不可以重复引用在同一个FROM子句中的表。例如：

select count(distinct c_id) from parents,
       table(parents.children) c_table(c_name, c_id)
        where parents.id = 1001;
5,数据库服务器必须静态指定隐含集合表达式的数据类型。
=======
1，不可以是INSERT， DELETE, UPDATE语句的目标。
2，不可以作为可更新的游标或视图以下的表。
3，不支持序列ordinality。
4，不可以重复引用在统一个FROM子句中的表。例如：
select count(distinct c_id) from parents,
       table(parents.children) c_table(c_name, c_id)
        where parents.id = 1001;
5,数据库服务器必须静态指定下集合表达式的数据类型。
>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c
6，The database server cannot support a reference to a host variable without
casting it to a known collection type. For example, rather than specifying
TABLE(:hostvar), you must cast the host variable:
TABLE(CAST(:hostvar AS type))
TABLE(CAST(? AS type))
<<<<<<< HEAD

如果不把宿主变量CAST为已知集合类型，则数据库服务器不支持对宿主变量的引用。
7，集合表类型LIST不可以保持行的顺序。
8，集合中的数据类型不可以是SERIAL，SERIAL8，BIGSERIAL

DECLARE集合类型变量的时候，显式指定CLIENT关键字。
=======
7，集合表类型LIST不可以保持行的顺序。
8，集合中的数据类型不可以是SERIAL，SERIAL8，BIGSERIAL

DECLARE集合类型变量的时候，必须显式指定CLIENT关键字。
>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c
1，显示指定collection变量：指定集合中元素的类型和集合自身类型。
EXEC SQL BEGIN DECLARE SECTION;
    CLIENT COLLECTION LIST(SMALLINT NOT NULL) LIST1;
    CLIENT COLLECTION SET(ROW(x CHAR(20),
                              y SET(INTEGER NOT NULL),
                              z DECIMAL(10,2)) NOT NULL) row_set;
    CLIENT COLLECTION MULTISET(SET(SMALLINT NOT NULL)
                               NOT NULL) collection3;
EXEC SQL END DECLARE SECTION;

<<<<<<< HEAD
/* Typed 集合类型 */
1,集合类型中的元素类型必须是内建类型。BYTE，TEXT，SERIAL，SERIAL8 都不可以。
且必须对元素类行进行NOT NULL限制。
2,只有SET和LIST可以创建嵌套集合。
3,命名的row类型不可以作为元素类型。然后，你可以指定与已命名row类型字段相同的元素类型。
4,opaque类型

当指定为collection变量的元素类型时，使用的是SQL数据类型，而不是ESQL/C数据类型
。如上例中的 SMALLINT，CHAR，DECIMAL。
=======
集合类型中的元素类型必须是内建类型。BYTE，TEXT，SERIAL，SERIAL8 都不可以。
且必须对元素类行进行NOT NULL限制。

(1)，命名的row类型不可以作为元素类型。然后，你可以指定与已命名row类型字段相同的元素类型。
>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c

CREATE ROW TYPE myrow
(
    a int,
    b float
);
CREATE TABLE mytable
(
    col1 int8,
<<<<<<< HEAD
    col2 set(myrow not null) 
);

EXEC SQL BEGIN DECLARE SECTION;
                    /* 未命名ROWL类型 */
client collection set(row(a int, b float) not null) my_collection;
EXEC SQL END DECLARE SECTION;

(2)，你可以显式定义一个与数据库集合列元素类型不同的集合，只要他们的元素类型可
以互相兼容。自动转换。
=======
    col2 set(myrow not null)
);
/* 未命名ROWL类型 */

EXEC SQL BEGIN DECLARE SECTION;
client collection set(row(a int, b float) not null)
my_collection;
EXEC SQL END DECLARE SECTION;

(2)，你可以显式定义一个与数据库集合列元素类型相同的集合，只要他们的元素类型可以互相兼容。
>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c


EXEC SQL BEGIN DECLARE SECTION;
    client collection set(float not null) set_float;
    client collection set(integer not null) set_int;
EXEC SQL END DECLARE SECTION;

EXEC SQL declare cur1 cursor for select * from tab1;
open cur1;
<<<<<<< HEAD
fetch cur1 into :set_float;
=======
fetch cur1 into:set_float;
>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c
fetch cur1 into :set_int; /* 第一次fetch后，定义了元素类型为float，所以此时产
                             生类型不匹配错误。*/




<<<<<<< HEAD
/* Untyped集合类型 */
元素类型和集合自身类型均不指定。
仅包含COLLECTION关键字和集合变量名字。
如果你不知道你想访问的集合元素的精确类型，就使用隐式collection变量。
=======

2，隐式指定：元素类型和集合自身类型均不指定。
仅包含COLLECTION关键字和集合变量名字。
如果你不知道你想access的集合元素的精确类型，就使用隐式collection变量。
>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c
你必须获取集合列的定义情况，以便使用未命名collection变量。

EXEC SQL BEGIN DECLARE SECTION;
client collection a_coll;


<<<<<<< HEAD
/* Client Collections */
当SQL语句包含一个collection变量，它有以下语法限制：
1，只可以通过在客户端使用集合衍生表 TABLE(:COLLEC)子句，与 SELECT INSERT
UPDATE DELETE 操作来获取集合元素。
2，INSERT 语句不能在VALUES子句中包含 SELECT，EXECUTE FUNCTION，或者 EXECUTE
PROCEDURE。
3，不能包含 WHERE 子句或表达式
4，不可以使用滚动游标
5，不能包含表达式。
=======
当SQL语句包含一个clollection变量，它有以下语法限制：
1，只可以通过在客户端使用集合衍生表 TABLE(:COLLEC)子句，与 SELECT INSERT
UPDATE DELETE 操作来获取元素。
2，INSERT 语句不能在VALUES子句中包含 SELECT，EXECUTE FUNCTION，或者 EXECUTE
PROCEDURE。
3，不能包含 WHERE 子句或表达式
4，不可以使用游标
>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c


ESQL/C并不自动分配或释放集合变量的内存，你必须显式指定。
当指向集合的游标处于OPEN状态时，DEALLOCATE COLLECTION 此集合失败

<<<<<<< HEAD
/* 集合列与集合变量的区别：*/
1，当你使用集合衍生表对集合变量进行操作时，语句并不发送给服务器进行处理，而是
ESQL/C处理。
2，当你对集合衍生表进行UPDATE或INSERT操作时，必须带有SET或VALUES子句。

/* 集合变量的初始化 */

必须总是对 Untyped集合变量进行初始化。采用如下语句：
SELECT 集合列 INTO 集合变量 FROM 表名或视图名（不可以是集合衍生表子句)

如果是Typed集合变量，还可以使用如下方法来初始化：
INSERT INTO TABLE(:col_var) VALUES (var);

EXEC SQL ALLOCATE COLLECTION :time_vals;
EXEC SQL SELECT TIME_DATASET INTO :time_vals
                                FROM readings
                                WHERE dataset_id = 1356;
=======
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
>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c
IFX_INT8CVINT(1423231, &an_int8);
EXEC SQL INSERT INTO TABLE(:time_vals) VALUES (:an_int8);

/* 插入游标 */
插入LIST集合类型时，可以使用 INSERT AT 3 INTO TABLE(:hostcollec)。。。。
插入游标允许你向集合变量插入多个值。但必须是顺序游标，不能是滚动游标和WITH HOLD游标。

EXEC SQL PREPARE ins_stmt FROM
<<<<<<< HEAD
        'INSERT INTO TABLE VALUES';
=======
        ’INSERT INTO TABLE VALUES’;
>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c
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

<<<<<<< HEAD
/* SELECT from集合变量 */
一，带有集合衍生表子句的SELECT语句允许从集合变量中SELECT元素。有以下限制：
1,SELECT 语句中的list不能包含表达式
2,SELECT 语句中的list必须是 '*'
3,以下SELECT子句和选项不被允许：GROUP BY，HAVING，INTO TEMP，ORDER BY，WHERE，
WITH REOPTIMIZATION
4,FROM 子句不能做连接

二，具体实现：
1,从集合中选择一个元素，事先必须确定只返回一个元素，否则使用SELECT游标:
SELECT * INTO :hostvar FROM table(:col_var);

2,从集合选择一行元素：
EXEC SQL BEGIN DECLARE SECTION;
client collection set(row(a integer) not null) a_set;
row (a integer) a_row;
EXEC SQL END DECLARE SECTION;
EXEC SQL select set_col into :a_set from tab1
                        where id_col = 17;
EXEC SQL select * into :a_row from table(:a_set);
3,选择多个元素
(1),对集合变量定义一个SELECT游标，限制如下：
集合SELECT游标是一个update游标。定义时不能包含FOR READ ONLY 子句
如果你想修改集合变量中的元素，定义一个SELECT游标作为UPDATE游标使用：
加上FOR UPDATE 子句。然后可以使用 DELECT 或 UPDATE 的WHERE CURRENT OF 子句删除
或更新集合元素。
(2),SELECT游标必须是顺序游标。

/* 更新集合变量 */
1,更新集合中所有元素为同一个值：
使用带有SET 衍生表子句，且不带WHERE CURRENT OF 子句的 UPDATE语句。

EXEC SQL BEGIN DECLARE SECTION;
client collection list(smallint not null) a_list;
int an_int;
EXEC SQL END DECLARE SECTION;
.
EXEC SQL update table(:a_list) (list_elmt) /*list_elmt为SET子句中集合元素的别名*/
set list_elmt = 16;

2,更新集合中某一个元素
$DECLARE xxxx CURSOR FOR SELECT * FROM TABLE(:col_var)
                for update;
$OPEN xxxx;
$FETCH xxxx INTO :hostvar;
如果HOSTVAR满足条件：
$UPDATE TABLE(:col_var)(x)
    SET x = 10
    WHERE CURRENT OF xxxx;

如果只是用字面值插入或更新集合列，不必使用collection宿主变量。

/* 删除集合中的元素 */
删除所有元素：
EXEC SQL DELETE FROM TABLE(:col_var);

删除单个元素：
定义一个FOR UPDATE 的SELECT游标。 FETCH。。INTO。。判定是否满足条件。如果满足
条件的话
DELECT FROM TABLE(:col_var) WHERE CURRENT OF cursor;

在ESQL/C程序中指定嵌套引号时，必须对在单引号字符串中的每一个双引号转义。

EXEC SQL insert into (col1) tab1
                values (’SET{\"abcde\"}’);
=======
/* SELECT游标 */

