smart large object类型存储大型的，非关系数据。包括CLOB(character large object)
    与BLOB(binary large object)数据类型。可以定位读写并支持更大的文件。
对应的数据类型为 ifx_lo_t。具体定义如下：

EXEC SQL include locator;
.
BEGIN DECLARE SECTION;
EXEC SQL fixed binary 'clob' ifx_lo_t clob_loptr;
EXEC SQL binary 'blob' ifx_lo_t blob_loptr;
END DECLARE SECTION;

/* 在创建一个新的 smart-large-object之前，必须使用ifx_lo_def_create_spec()函数
 * 来为LO规格的结构——ifx_lo_create_spec_t 分配内存。这个结构比较难懂，且其实
 * 现和功能可能会在未来改变，所以要尽量使用ESQL/C函数来读取和改变它的值。 */
LO-规格结构包含两部份信息：1,磁盘保管信息， 2,创建时间参数

<<<<<<< HEAD
/* 磁盘保管信息 */

=======
>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c
预估字节大小        服务器使用这个信息探测对象大小。ifx_lo_specget_estbytes()
                提供一个优化信息。如果值不符，服务器ifx_lo_specset_estbytes()
                仍不产生错误，会使用合适的值。

最大字节大小                                        ifx_lo_specget_maxbytes()
                                                    ifx_lo_specset_maxbytes()

分配范围大小    KB单位，通常是一个CHUNK的大小。     ifx_lo_specget_extsz()
                                                    ifx_lo_specset_extsz()

sbspace的大小   包含smart LO对象的名字，最多18字节  ifx_lo_specget_sbspace()
                                                    ifx_lo_specset_sbspace()

<<<<<<< HEAD
/*创建时间参数 */

指示类型    创建时间参数        说明
/*******************************************************************/
Logging     LO_LOG      在日志文件记录SMART LO的改变。慎重使用，日志容量要大
            LO_NOLOG

Last access-time LO_KEEP_LASTACCESS_TIME 服务器存储最后读取时间。慎重使用，开
                                            销很大
                 LO_NO_KEEP_LASTACCESS_TIME
ifx_lo_specset_flags();
ifx_lo_specget_flags();

ifx_lo_col_info();   /* 用列级保管特性更新LO规格结构体 */
ifx_lo_create();    /* 为一个新创建的对象读取包含保管特性的LO规格结构体 */
ifx_lo_spec_free(); /* 释放资源 */

/* 列级保管特性 */
DBA使用 CREATE TABLE 对列级保管特性赋值。PUT 子句指定一个 CLOB、BLOB 列的保管
特性。syscolattribs 系统目录表存储特性。

为打开一个smart LO进行读写操作，ESQL/C程序必须有一个LO指针结构体。此结构体包含
磁盘地址和一个惟一的十六进制标识符。

ifx_lo_copy_to_file();  /* 函数创建一个新的smart LO的LO指针结构体。ifx_lo_t */
ifx_lo_create();        /* 初始化指向新smart LO的LO指针结构体。创建并返回一个
                           LO描述符 */
ifx_lo_filename();      /* 返回ifx_lo_copy_to_file()函数存储的smart LO指针标识符*/
ifx_lo_from_buffer();   /* 从用户自定义缓冲区复制指定的字节大小到smart LO指针
                           引用的LO对象 */
ifx_lo_to_buffer();

/* LO文件标识符 */
LO文件标识符是一个整形值，用以标识smart LO。
ifx_lo_readwithseek();
ifx_lo_seek();
ifx_lo_stat();
ifx_lo_tell();
ifx_lo_truncate();
ifx_lo_write();
ifx_lo_writewithseek();

/* 创建 smart LO的步骤 */
1,分配LO规格的结构体内存。 ifx_lo_def_create_spec()
2,确保LO规格结构体包含你想要的保管特性。
3,为新LO创建一个LO指针结构体，并打开LO对象。ifx_lo_create()
4,ifx_lo_write()或ifx_lo_writewithseek() 写入内容。
5,在数据库列中存储LO对象。

/* 访问smart LO */
1,从数据库 SELECT smart LO对象 INTO ifx_lo_t类型的宿主变量。
2,ifx_lo_open()
3,进行读写操作，更新smart LO内容
4,ifx_lo_close()

/* ifx_lo_open和ifx_lo_create使用到的参数 */
LO_RDONLY
LO_DIRTY_READ 只限ifx_open()使用。允许你读取不受约束的数据页。此选项不可用于写
LO_WRONLY
LO_APPEND   要与LO_WRONLY或LO_RDWR结合使用。如果单独使用，是只读打开LO对象。
LO_RDWR
LO_BUFFER   使用基本的数据库缓冲池
LO_NOBUFFER 轻量级IO，使用数据库服务器进程池中的私有缓冲区
LO_LOCKALL
LO_LOCKRANGE

使用 LO_NOBUFFER的注意事项：
结束对LO的操作之后，一定使用ifx_lo_close()函数
所有使用轻量级I/O打开的smart LO共享同样的私有缓冲区。对缓冲区中页的操作可能会
被刷新。

如果smart LO还没有打开，可以使用 ifx_lo_alter() 函数将smart LO从轻量级I/O切换
回BUFFERD I/O。然而，相反的操作会导致错误。
LO_NOBUFFER不能用于写文件

/* 打开一个smart LO的持续时间 */
用ifx_lo_create()和ifx_lo_open()函数打开的文件，只有在你使用了ifx_lo_close()函
数或者会话结束后才被关闭。
即使传输内容结束，也不会关闭噢。

/* 删除一个smart LO */
只有在两种情况都达成的情况下，smart LO才被删除：
1,当前传输被提交
2,打开的smart LO关闭之后

/* 分配、访问一个LO状态结构 */

LO状态结构的函数：

ifx_lo_stat(); /*分配并初始化LO文件描述符的LO状态结构 */
ifx_lo_stat_atime(); /* 事先要设定LO_KEEP_LASTACCESS_TIME。smart LO最后访问的
                        时间。*/
ifx_lo_stat_cspec(); /* smart LO的状态特性。这些特性存储在LO规格结构体中。 */
ifx_lo_stat_ctime(); /* smart LO状态的最后改变时间，如修改，更改所有者和引用人数 */
ifx_lo_stat_mtime_sec(); /* 最后修改时间，如写入/创建 */
ifx_lo_stat_refcnt();   /* smart LO的引用数 */
ifx_lo_stat_size();     /* smart LO的字节大小 */

/* 修改 smart LO列 */
使用 ALTER TABLE 的 PUT 子句更改CLOB列或BLOB列的保管定位和特性。
你可以改变列存储的sbspace，
/*
 *
 *
 *
 *
 *
 *
 *
 * 这里看不懂。可参照demo/esqlc目录下的 create_clob.ec get_lo_info.ec
 * upd_lo_descr.ec 了解。 */

/* 迁移 simple LO 到 smart LO */

update catalog set advert = ROW ((SELECT cat_picture::blob
                                  FROM stores7:catalog WHERE catalog_num = 10027), pwd
                                 advert.caption)
                WHERE catalog_num = 10027;


ALTER TABLE catalog modify cat_descr CLOB,
      PUT cat_descr in (sbspc);
=======

>>>>>>> ccc18beee2a7ae181a27dde87dd9a700e27ba11c
