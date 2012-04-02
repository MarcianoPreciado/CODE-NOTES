/* 第八章：扩充杂类函数组头文件
 */

/******************************************
函数功能:取ini文件中设置项的值
函数参数: filename  文件名
          cfg_seg   段名
          cfg_item  设置项名称
          value     设置项的值
          vsize     缓冲区长度
函数返回: 0: 成功
 *         -1: 配置文件不存在
 *         -2: 未找到配置节
 *         -3: 未找到配置项
 ******************************************/
int ExGetCfgItem(char* filename,char *cfg_seg,
                 char *cfg_item, char *value, int vsize);

int GetCfgItem(char *cfg_seg, char *cfg_item, char *value); /* 相关环境变量:HSETCDIR=$HOME/etc*/
void SetIniName(char *filename);  /* 和GetCfgItem配套使用 */

int daemon_start(int ignsigcld);/* 进程转守护进程。ignsigcld为是否忽略子进程状态 */

/* 输入参数:
 *          char  *cmdname  进程名
 *          int   scope     方式 0 仅检查当前用户进程 1 检查所有用户进程
 *          int   verbose   提示方式  0 不需要提示信息 1 标准方式输出信息
 *          int   *runnings 输出运行进程数
 * 返    回:
 *          -1  执行错误
 *          >=0 活动进程数
 */
int checkdeamon(char *cmdname, int scope);
int closedeamon(char *cmdname,int scope,int verbose,int *runnings);

