/* 第九章：XML函数
 */

#include "xml.h"
/* szRootName为XML结构根源素的名称。不再使用时必须调用xml_Destroy销毁。成功时
   返回XML句柄，否则返回FAIL。
 */
HXMLTREE xml_Create( const char *szRootName);

int xml_Destroy(HXMLTREE hXML);

int xml_Clear(HXMLTREE hXML); /* 清除除根结点外的所有数据。成功SUCC，否则FAIL */


/* 
 * ===  FUNCTION  =============================================================
 *         Name:  xml_AddElement
 *  Description:  在XML树中添加一个叶子结点。
 *  叶子结点可以重复存在。添加时添加如果路径结点不存在，则连同这些结点一起添加
 *  
  hXML
    指定XML设备句柄
  szElementName
    指定在XML结构要添加的元素的名称，形如"/PRIVATE|2/电费单价|0"
    "/NAME1|IDX1/NAME2|IDX2/.../NAMEn|IDXn"
    NMAE  在该层上的节点元素名，
    IDX    指定每一层元素在同名兄弟元素中的排序位置。
      如果iIndex为0，表示添加在最后
      如果为1，2，3……，表示取相应编号的元素。
      缺省为0
  szElementValue
    待添加的元素值指针
    节点分为数据节点和属性节点
    如果该参数有字串值，则该节点为数据节点，
    如果该参数值为""或NULL，则该节点可以为属性节点，以后该节点可以添加属性。
 *  返回值:
  成功  返回SUCC
  失败  返回FAIL
 * ============================================================================
 */
int xml_AddElement(HXMLTREE hXML, const char *szElementName, const char *value);
int xml_SetElement( HXMLTREE hXML, const char *szElementName, const char
                   *szElementValue );
int xml_GetElement( HXMLTREE hXML, const char *szElementName,
       char *pucElementBuff, const size_t uiBuffSize ); /* 成功则返回实际字节数 */

int xml_ModifyElement( HXMLTREE hXML, const char *szElementName,
       const char *szElementValue );

int xml_DelElement( HXMLTREE hXML, const char *szElementName );

int xml_ElementExist( HXMLTREE hXML, const char *szElementName ); /* 如果存在返回1,不存在返回0 */

int xml_IsLeafNode( HXMLTREE hXML, const char *szElementName );

int xml_ChildElementCount( HXMLTREE hXML, const char *szElementName );

int xml_GetChildElementName(HXMLTREE hXML, const char *elementname,
       const int iPos,
       char *buf, const size_t bufsize);

/*********************************************************************
 *  函数:  xml_AddElementAttr
 *  功能:  在XML树中添加一个节点属性
    属性节点不可以重复存在 
    ******************************************************************/
int xml_AddElementAttr( HXMLTREE hXML, const char *szElementName,
       const char *szAttributeName, const char *szAttributeValue );

/*     如果节点属性不存在，则填加一个节点属性；
 *     如果节点属性存在，则修改其值为新值
 */
int xml_SetElementAttr(HXMLTREE hXML, const char *szElementName,
       const char *szAttributeName, const char *szAttributeValue);

int xml_GetElementAttr(HXMLTREE hXML, const char *szElementName,
       const char *szAttributeName,
       char *pucAttributeBuff, const size_t uiAttrBuffSize );

int xml_DelElementAttr(HXMLTREE hXML,
       const char *szElementName, const char *szAttributeName);

int xml_ElementAttrExist(HXMLTREE hXML,
       const char *szElementName, const char *szAttributeName);

int xml_ModifyElementAttr( HXMLTREE hXML, const char *szElementName,
       const char *szAttributeName, const char *szAttributeValue );

int xml_GetAttributeName( HXMLTREE hXML, const char *szElementName,
       const int iPos, char *pucAttrNameBuff, const size_t uiAttrNameBuffSize );

int xml_AttributeCount( HXMLTREE hXML, const char *szElementName );

int xml_ElementCount( HXMLTREE hXML, const char *szElementName );

/**********************************************************************
 *  函数:  xml_ExportXMLString
 *  参数:
  szExportName
    指定XML结构导出位置的元素的名称，形如"/PRIVATE|2/电费单价|0"
    如果为NULL或""，则导出所有数据
  bNodeSelf
    是否包含节点自身数据
    非0    导出时包含该节点元素。
    0    导出时导出该节点所有子节点数据，不包含该节点本身。
  *  返回值:
  成功  返回导出的字节数
  失败  返回FAIL
 **********************************************************************/
int xml_ExportXMLString( HXMLTREE hXML, char *pucXMLBuff,
      size_t uiXMLBuffLen, const char *szExportName, int bNodeSelf );

/*   szImportXMLString
 *     XML格式字串指针
 *   szImportName
 *     指定在XML结构接收导入的位置的元素的名称，形如"/PRIVATE|2/电费单价|0"
 *     如果为NULL或""，则导入在根节点下
 *   bReplace
 *     是否替换szImportName节点
 *     >0 替换 用XML字串中第一层上从左到右顺序指定位置节点替换
 *     <0 替换 用XML字串中第一层上从右到左顺序指定位置节点替换
 *     0  不替换,所有数据引入到该节点下
 */


int xml_ImportXMLString(HXMLTREE hXML,
      const char *szImportXML,
      const char *szImportName,
      int bReplace);

int xml_GetLastError(void);
char *xml_StringError(int iXMlErrno);

size_t xml_asctohex( char *szHexBuf, size_t size, char *szAsc, size_t num ); /* 失败返回0 */
size_t xml_hextoasc( char *szAscBuf, size_t size, char *szHex, size_t num );


