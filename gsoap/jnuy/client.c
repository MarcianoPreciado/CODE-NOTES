#include "soapH.h"
#include "yl_USCOREydSoap.nsmap"
#include "soapList.h"
#include <string.h>
#include <stdlib.h>


/**
 * @brief   遍历DOM元素，将其下的所有内容依次存入soapList链表
 * @param   domEle 一般传参为是gSOAP解析生成的DOM root节点
 * @param   header soapList链表头节点
 * @param   链表中存储的当前domEle的parent节点
 */
void travelDom(const struct soap_dom_element *domEle, struct soapList *header,
               struct soapList *parent, const char *prefix)
{
  const struct soap_dom_element *iter = domEle;
  for (; iter != NULL; iter = iter->next){
    char *strTmp = calloc(1024, sizeof(char));

    if (prefix != NULL) {
      strncpy(strTmp, prefix, 1000);
      strTmp[1000] = '\0';
      strcat(strTmp, ":");
    }

    strncat(strTmp, iter->name, 1024 - sizeof(strTmp) - 1);

    struct soapList *node = soapList_insert(header, parent, strTmp, iter->data);
    if (iter->elts != NULL)
      travelDom(iter->elts, node, node, strTmp); /* 这里travelDom的header参数可以赋值为node，
                                                    减少找到插入位置（NULL）的算法时间。 */
    free(strTmp);
  }
}

int main(int argc, char *argv[])
{
  /*  以下三行为SOAP初始化，
   *  SOAP_IO_KEEPALIVE参数保持连接，防止多次SOAP操作造成多次重复连接
   *  SOAP_C_UTFSTRING设定为接收和发送的字符串为UTF-8编码
   *  */
  struct soap soap;
  soap_init1(&soap, SOAP_XML_INDENT | SOAP_IO_KEEPALIVE | SOAP_XML_DOM);
  soap_set_mode(&soap, SOAP_C_UTFSTRING);

  /* GetTime调用，客户端每种调用以 soap_call__(双下划线）开头，后面为类名
   * 具体类成员可以参看以下路径文件 html/annotated.html
   * 如 _ns1__GetTime
   *
   * 返回值则为相应的 _ns1__GetTimeResponse，均以Response结尾，具体成员同样参照
   * annotated.html
   *
  *http://public.jinanwater.com.cn/websvr/yhds/yl_yd.asmx/ 页面中的调用命令如有 _（单下环线），
  则_替换为 _USCORE
   *
   */
  struct _ns1__GetTimeResponse curTime;
  curTime.
  soap_call___ns1__GetTime(&soap, NULL, NULL, NULL, &curTime);

  /* 判断是否接收错误，如果错误打印错误信息到stderr */
  if (soap.error) {
    soap_print_fault(&soap, stderr);
    exit(1);
  }
  else {
    printf("it's ok\n");
    printf("Gettime timeresult is %lld\n",(long long int) curTime.GetTimeResult);
  }


  /* 测试hbwsh调用。
  */

  /* 每次调用都要分配一个soapList链表头的空间，用于接收数据 */
  struct soapList *header1 = calloc(1, sizeof(struct soapList)); /* 用calloc可以减去
                                                                    设定指针为NULL，字符串为空的步骤 */
  struct _ns1__get_USCOREhbwsh hbwsh;
  hbwsh.gs = "1000008";
  hbwsh.id = "123";
  hbwsh.yyzd = "5-兴业银行";

  struct _ns1__get_USCOREhbwshResponse hbwshResponse;
  soap_call___ns1__get_USCOREhbwsh(&soap, NULL, NULL, &hbwsh, &hbwshResponse);
  struct soap_dom_element any = hbwshResponse.get_USCOREhbwshResult->__any;
  if (soap.error) {
    soap_print_fault(&soap, stderr);
    exit(1);
  }
  printf("\n\n测试hbwsh with: gs=1000008, id = 123, yyzd = 5-兴业银行\n");
  travelDom(&any, header1, NULL, NULL);

  /*  soapList打印内容，为用于后续操作，可以将这个header指针返回给其他函数，用于操
   *  作*/
  soapList_printAll(header1);
  /* @note 析构释放空间 */
  soapList_emptyList(header1);


  /*  测试hbwsh有效ID调用 */
  struct soapList *header2 = calloc(1, sizeof(struct soapList));
  hbwsh.gs = "1000005";
  hbwsh.id = "123";
  hbwsh.yyzd = "5-兴业银行";
  printf("\n\n测试hbwsh with: gs=1000005, id = 123, yyzd = 5-兴业银行\n");

  soap_call___ns1__get_USCOREhbwsh(&soap, NULL, NULL, &hbwsh, &hbwshResponse);
  any = hbwshResponse.get_USCOREhbwshResult->__any;
  if (soap.error) {
    soap_print_fault(&soap, stderr);
    exit(1);
  }
  travelDom(&any, header2, NULL, NULL);

  soapList_printAll(header2);
  /*  析构释放空间 */
  soapList_emptyList(header2);

  /* 以下三句操作完成后释放SOAP分配的空间 */
  soap_destroy(&soap);
  soap_end(&soap);
  soap_done(&soap);

  return 0;
}
