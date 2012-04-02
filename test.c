#include <stdio.h>
#include "xml.h"
int main(void)
{
    HXMLTREE
    hXML;
    int
    ret;
    char
    szBuf[1024];
    /* 构造 XML 分析设备 */
    if ( (hXML = xml_Create("Root") == FAIL )) {
        exit(-1);
    }
    /* 添加属性节点 */
    ret = xml_AddElement( hXML, "/PRIVATE/FONT", NULL);
    ret = xml_AddElement( hXML, "/PRIVATE/FONT", NULL);
    /* 添加元素节点 */
    ret = xml_AddElement( hXML, "/PRIVATE/FONT|1/GROUP1", "DATA1");
    ret = xml_AddElement( hXML, "/PRIVATE/FONT|2/GROUP1", "DATA2");
    ret = xml_AddElement( hXML, "/PRIVATE/FONT|1/GROUP2", "DATA3");
    ret = xml_AddElement( hXML, "/PRIVATE/FONT|2/GROUP2", "DATA4");
    ret = xml_AddElement( hXML, "/PRIVATE/FONT/GROUP1", "DATA5");
    /* 添加属性 */
    ret = xml_AddElementAttr( hXML, "/PRIVATE/FONT", "name", "Arial");
    ret = xml_AddElementAttr( hXML, "/PRIVATE/FONT", "color", "red");
    /* 导出字串 */
    ret = xml_ExportXMLString(hXML, szBuf, sizeof(szBuf), NULL);
    printf("Length=[%d]\n", ret);
    printf("szBuf=**[%s]**\n", szBuf);
    /* 销毁 XML 分析设备 */
    xml_Destroy(hXML);
    return 0;
}
