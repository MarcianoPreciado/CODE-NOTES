/* 第10章：组件开发扩展宏头文件
 */

/* 组件辅助宏  RXDA元素硬读取(读取错误退出函数，并记录相关日志)
 * 限    制:
 *   buf  必须为字符串，不可为字符串指针
 */
COMP_HARDGETXML(nodename, buf);

/* 组件辅助宏  RXDA元素软读取(读取错误不退出函数,只记录相关日志)
 */
COMP_SOFTGETXML(nodename, buf);

COMP_HARDSETXML(nodename, buf);
COMP_SOFTSETXML(nodename, buf);
COMP_HARDADDXML(nodename, buf);
COMP_SOFTADDXML(nodename, buf);

/* 组件辅助宏 比照允许参数数,获取组件参数个数
 * 输入参数:
 * paracount 允许参数个数
 * 限 制:
 * 需预先定义变量 int iParas

 */
COMP_PARACOUNTCHK(paracount);
COMP_PARACOUNTCHKMIN(paracount);

/* 组件辅助宏  获取组件普通参数(建议使用COMP_GETPARAS)
 * 输入参数: 
 *   parasn      参数序号
 *   parabuf     参数缓冲区
 * 限    制:
 *   parabuf  必须为字符串变量，不可为字符串指针
 */
COMP_GETPARA(parasn, parabuf);

COMP_GETPARSEPARA(parasn, parabuf); /* 获取组件可解析参数 */

COMP_GETPARAS(parasn, parabuf, paraname); /* 获取组件普通参数 */
COMP_GETPARSEPARAS(parasn, parabuf, paraname); /* 获取组件可解析参数 */
