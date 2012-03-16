/* Working with simple large objects */

simple large object包括TEXT和BYTE数据类型。
其中，TEXT数据类型存储文本数据。BYTE数据类型可以在一致的字节流中存储任何形式
的二进制数据。

simple large object最大支持2GB的数据，读写时要么全读全写，要么没有成功。
smart large object最大值吃4TB数据，可以设定指定的部份进行读写操作。

simple在ESQL/C中的数据类型为loc_t，不可以直接INSERT INTO一个simple large
object，需要使用LOAD语句或者loc_t宿主变量类型。
loc_t并不包含真正的数据，只是一个定位结构体。推荐：在使用它之前初始化数据，
如下：

EXEC SQL include locator;
byfill(&blob1, sizeof(loc_t), 0);
where blob1 is declared as
EXEC SQL BEGIN DECLARE SECTION;
loc_t blob1;
EXEC SQL END DECLARE SECTION;

/*
Locators are used to store TEXT or BYTE fields (blobs) in ESQL
programs.

If "loc_loctype" is set to LOCMEMORY, then the blob is stored in
primary memory.  The memory buffer is pointed to by the variant
"loc_buffer".  The field "loc_bufsize" gives the size of "loc_buffer".
如果设定为正值，ESQL/C假设程序自己分配内存缓冲区并析构。


如果loc_bufsize设定为-1, 且loc_mflags为0，ESQL/C程序会分配内存以存放
simple-large-object数据。如果分配内存失败，设定"loc_status"字段为-465以指示错
误。
select或者第一次fetch成功后，ESQL/C设定"loc_buffer","loc_bufsize","loc_size"为合适的值。
如果随后要fetch的simple-large-objects过大或过小，要设定loc_mflags为 LOC_ALLOC
常量。使ESQL/C重新分配一个新的内存缓冲区。如果新fetch的数据增大，ESQL/C释放掉
现在的缓冲区并分配相近的缓冲区，重新设定loc_buffer,loc_bufsize和loc_size。

If "loc_loctype" is set to LOCFILE, then the blob is stored in a file.
The file descriptor of an open operating system file is specified in
"loc_fd".

If "loc_loctype" is set to LOCFNAME, the the blob is stored in a file
and the name of the file is given.  The DBMS will open or create the
file at the correct time and in the correct mode.

If the "loc_loctype" is set to LOCUSER, "loc_(open/close/read/write)"
are called.  If the blob is an input to a SQL statement, "loc_open" is
called with the parameter "LOC_RONLY".  If the blob is an output target
for an SQL statement, "loc_open" is called with the parameter
"LOC_WONLY".

"loc_size" specifies the maximum number of bytes to use when the
locator is an input target to an SQL statement. It specifies the number
of bytes returned if the locator is an output target.  If "loc_loctype"
is LOCFILE or LOCUSER, it can be set to -1 to indicate transfer until
end-of-file.

"loc_indicator" is set by the user to -1 to indicate a NULL blob.  It
will be  set to -1 if a NULL blob is retrieved.  If the blob to be
retrieved will not fit in the space provided, the indicator contains
the size of the blob.

"loc_status" is the status return of locator operations.

"loc_type" is the "blob" type (SQLTEXT, SQLBYTES, ...).

"loc_user_env" is a pointer for the user's private use. It is neither
set nor examined by the system.  "loc_user_env" as well as the
"loc_union" fieds may be used by user supplied routines to store and
communicate information.
*/

typedef struct tag_loc_t
    {
    int2 loc_loctype;		/* USER: type of locator - see below	*/
    union			/* variant on 'loc'                     */
	{
	struct			/* case LOCMEMORY                       */
	    {
	    int4  lc_bufsize;	/* USER: buffer size */
	    char *lc_buffer;	/* USER: memory buffer to use		*/
	    char *lc_currdata_p;/* INTERNAL: current memory buffer，不能被ESQL/C修改*/
	    mint   lc_mflags;	/* USER/INTERNAL: memory flags		*/
				/*			(see below)	*/
	    } lc_mem;

	struct			/* cases L0CFNAME & LOCFILE		*/
	    {
	    char *lc_fname;	/* USER: file name			*/
	    mint  lc_mode;	/* USER: perm. bits used if creating	*/
	    mint  lc_fd;	/* USER: os file descriptior		*/
	    int4  lc_position;	/* INTERNAL: seek position		*/
	    } lc_file;
	} lc_union;

    int4  loc_indicator;	/* USER SYSTEM: indicator		*/
    int4  loc_type;		/* USER SYSTEM: type of blob		*/
    int4  loc_size;		/* USER SYSTEM: num bytes in blob or -1	*/
    mint  loc_status;		/* SYSTEM: status return of locator ops	*/
    char *loc_user_env;		/* USER: for the user's PRIVATE use	*/
    int4  loc_xfercount;	/* INTERNAL/SYSTEM: Transfer count	*/

    mint (*loc_open)(struct tag_loc_t *loc, mint flag, mint bsize); /*成功返回0,否
则返回-1，loc_status（SQLCODE）设为-452 */
    mint (*loc_close)(struct tag_loc_t *loc);/*失败的话返回
                                               -1,sql_status(SQLCODE)设为 -453*/
    mint (*loc_read)(struct tag_loc_t *loc, char *buffer, mint buflen); (SQLCODE-454)
    mint (*loc_write)(struct tag_loc_t *loc, char *buffer, mint buflen);-455

    mint   loc_oflags;		/* USER/INTERNAL: see flag definitions below */
	} ifx_loc_t;

    #define loc_fname	lc_union.lc_file.lc_fname
    #define loc_fd		lc_union.lc_file.lc_fd
    #define loc_position	lc_union.lc_file.lc_position
    #define loc_bufsize	lc_union.lc_mem.lc_bufsize
    #define loc_buffer	lc_union.lc_mem.lc_buffer
    #define loc_currdata_p	lc_union.lc_mem.lc_currdata_p
    #define loc_mflags	lc_union.lc_mem.lc_mflags

    /* loc_oflags选项 */
    #define LOC_RONLY	0x1		/* read only */
    #define LOC_WONLY	0x2		/* write only */
    #define LOC_APPEND	0x4		/* write with append */
    #define LOC_TEMPFILE	0x8		/* 4GL tempfile blob */

/* 所有这些参数都通过loc_open() 函数打开文件。如果ESQL/C读写文件失败，设定
 * loc_status字段为-463或-464。如果不能关闭文件，设定loc_status字段为-462。
 * SQLCODE值等同于此。*/

/* 在数据库表中的simple-large-objects列并不真正包含对象数据。只包含一个56字节
 * 的simple-large-objects描述符，用以定位存储数据的第一段。这个描述符可以指向
 * 一个dbspace blobpage，一个blobspace blobpage或者一个存储系统的一部份。
 *
 * 当数据被保存在一张CD-R时，可以将loc_oflags字段设为LOC_DESCRIPTOR，只处理
 * simple-large-objects描述符 */


/* 本章例子文件参看 $INFORMIX/demo/esqlc/目录下的getcd_me.ec 和 updcd_me.ec
 * dispcat_pic.ec文件
 * 尤其要注意loc_indicator的指示NULL作用。
 * */

