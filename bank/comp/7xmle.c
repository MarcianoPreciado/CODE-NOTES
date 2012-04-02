/* 第七章：GAPS XML操作补充函数组
 */
#include "gaps_xml.h"
/* 以下函数成功返回SUCC，失败返回FAIL */
/* 函数功能:
 * 对XML数据访问封装,根据路径访问节点或属性，支持GAPS应用相对路径
 * 输入参数: 
 *     hXML
 *                 指定XML设备句柄
 *     szElementName
 *           
 *       节点路径表示:
 *       
 *         指定在XML结构要获取的元素的名称，形如"/PRIVATE|2/电费单价|0"
 *       属性路径表示:
 *         先由节点指定规则指定在XML结构中的节点。
 *         在通过@符号指定节点的属性。
 *         形如"/PRIVATE|2/电费单价@单位
 *         "/PRIVATE|2/电费单价"指定节点路径，"@单位"指定节点属性。
 */
int xml_RelGetElement(HXMLTREE hXML, const char *szElementName,
                      char *pucElementBuff, const size_t uiElementBuffSize);

int xml_RelSetElement( HXMLTREE hXML,
                   const char *szElementName, const char *szElementValue );

/* 设置XML中项目值，转换实际ASCII内容数据以字符hex存储
 * 支持GAPS应用相对路径 */
int xml_RelSetBinElement(HXMLTREE hXML, const char *szElementName, char *szElementValue, int szElementLen);

/* 把XML中以字符hex存储的内容转换为实际的ASCII内容数据，支持相对路径 
 * 
    返回: 
            >=0 取到值,并放入缓冲的长度
            <0 失败
 */
int xml_RelGetBinElement(HXMLTREE hXML,const char *szElementName,
                         char *pucElementBuff,const size_t uiElementBuffSize );

/* 处理输入的串中的XML项目，用确定值进行替换
   如果'[' 间所指节点不存在则返回‘’.符号'\'为'[',']'的转意符。
 */
int xml_RelParseXMLString(HXMLTREE hXML, char *sexp, int isize);

/* 判断XML树中指定节点是否存在
 * 如存在，返回值；否则返回0 */
int xml_RelElementExist( HXMLTREE hXML, const char *szElementName );

/* 统计XML树中指定节点元素同一层上的个数 */
int xml_RelElementCount( HXMLTREE hXML, const char *szElementName );

