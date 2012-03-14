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

预估字节大小        服务器使用这个信息探测对象大小。ifx_lo_specget_estbytes()
                提供一个优化信息。如果值不符，服务器ifx_lo_specset_estbytes()
                仍不产生错误，会使用合适的值。

最大字节大小                                        ifx_lo_specget_maxbytes()
                                                    ifx_lo_specset_maxbytes()

分配范围大小    KB单位，通常是一个CHUNK的大小。     ifx_lo_specget_extsz()
                                                    ifx_lo_specset_extsz()

sbspace的大小   包含smart LO对象的名字，最多18字节  ifx_lo_specget_sbspace()
                                                    ifx_lo_specset_sbspace()


