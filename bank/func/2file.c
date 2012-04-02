/* 第二章：配置文件操作函数
 */
int ini_SetUpdateMode(int iUpdateMode); /* 1,立即修改。0,关闭时修改 */

int ini_OpenIniFile(char *Filename);
/* 打开配置文件，如果文件已存在，则将其信息读入链表，否则创建新文件
*/
int ini_CloseIniFile(void);

ini_ReadKeyAsString(char *, char *, char *, char *, int);
/* 函数功能:从树中读取指定段中指定键的值, 如不存在则初始化为 sDefaultValue
 * 输入参数:段名字符串指针, 键名字符串指针, 默认值, 值字符串指针
 */

int ini_WriteKeyAsString(char *, char *, char *, char *);
/* 函数功能:写树中指定段下的指定键, 如果键不存在则新建
 * 输入参数:段名字符串指针, 键名字符串指针, 值字符串指针
 */

int ini_ReadEncrypt(char *, char *, char *, int);
int ini_WriteEncrypt(char *, char *, char *, char *);

int ini_ReadKeyAsEnv(char *, char *, char *, int);
/* 函数功能:读取指定段下指定键的值, 并解释其中的环境变量
 * 输入参数:段名称字符串指针, 键名字符串指针, 返回值字符串指针, 返回值长度
 */

int ini_AddSection(char *sSectionName, char *sComment);
/* 函数功能:在链表中增加一个段节点
 * 输入参数:段名字符串指针, 注释字符串指针
 */

int ini_AddKey(char *, char *, char *, char *);
/* 函数功能:在链表中某个段下增加一个键节点
 * 输入参数:段名字符串指针, 键名字符串指针, 键值字符串指针, 注释字符串指针
 */

int ini_RenameSection(char *, char *)
/* 函数功能:改变指定段的名称,如果新名称的长度超过最大长度, 则截断
 * 输入参数:旧段名字符串指针, 新名称字符串指针
 */

int ini_RenameKey(char *, char *, char *);
/* 函数功能:改变指定键的名称,如果新名称的长度超过最大长度, 则截断
 * 输入参数:段名称字符串指针, 旧键名字符串指针, 新名称字符串指针
 */

int ini_DeleteSection(char* );
int ini_DeleteKey(char *, char *);

#include <des.h>
#define   MAC_HS    0
#define   MAC_STAND         1
#define   MAC_BPI           2

/* MAC算法:
   志远算法:
       串"00000000"与数据进行8字节异或, 若数据长度不为8的整数倍, 则添
       空字符0x00, 将异或的结果与当前密钥进行DES运算生成.
   Stand:
       串"\0\0\0\0\0\0\0\0"与数据进行8字节异或, 异或的结果进行DES运算作为
       下次异或的初值, 若数据长度不为8的整数倍, 则添空字符0x00.
   BPI:
       串"\0\0\0\0\0\0\0\0"与数据进行8字节异或, 若数据长度不为8的整数倍,
       则添空字符0x00, 将异或的结果与当前密钥进行DES运算生成.
*/
/*生成MAC校验*/
char *GenerateMAC(int method, char *databuf, int datalen, char *key, char *MAC);

/* DES算法
   参数:
      op       -- 0,加密; 非零, 解密
      key      -- 密钥
      inblock  -- 源串
      outblock -- 目的串
      len      -- 加/解密的串长度
   返回: 实际加/解密的长度
   注意:
       len最好为8的整数倍, outblock长度必须大于等于len
*/
int DES(int op, char *key, char *inblock, char *outblock, int len);

