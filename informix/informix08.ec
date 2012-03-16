
typedef struct sqlca_s
    {
    int4 sqlcode;  /* 执行成功 */
#ifdef DB2CLI
    char sqlerrm[600]; /* error message parameters */
#else /* DB2CLI */
    char sqlerrm[72]; /* error message parameters */
#endif /* DB2CLI */
    char sqlerrp[8];  /* 目前未用 */
    int4 sqlerrd[6];
		    /* 0 - 返回记录的数目 */
		    /* 1 - insert操作后的序列值，或ISAM错误代码（更详细的解释） */
		    /* 2 - 处理的记录行数 */
		    /* 3 - 估计的代价（动态SQL中使用） */
		    /* 4 - SQL语句发生错误的偏移值 */
            /* 5 - insert操作后的ROWID  */
#ifdef _FGL_
    char sqlawarn[8];
#else
    struct sqlcaw_s
	{
    /* 执行成功，但有警告时设定 */
	char sqlwarn0; /* = W sqlwarn[1-7]被设置一个或多个 */
	char sqlwarn1; /* = W into宿主变量时被截断或打开有执行事物的数据库或被
                      REOVOKE的权限并没有收回 */
	char sqlwarn2; /* = W 聚集函数返回NULL值或ANSI DATABASE被打开 */
	char sqlwarn3; /* = W select字段列表的项数与INTO子句宿主变量数量不一样或
                      打开一个Online数据库或没有被赋予权限。*/
	char sqlwarn4; /* = W DESCRIBE一个包含DELETE的REPARE语句，或UPDATE语句中噢
                语句中没有包含WHERE子句。*/
	char sqlwarn5; /* = W 非ANSI标准的语句 */
	char sqlwarn6; /* = W if server is in data replication secondary mode */
	char sqlwarn7; /* = W 保留 */
	} sqlwarn;
#endif
    } ifx_sqlca_t;

SQLCODE < 0, 运行时错误
SQLCODE = 0, 执行成功
SQLCODE = 100，没有找到记录

