/* 第五章：IPC处理函数
 */
#include "hsipc.h"

int DoInitMsg(key_t Msgkey);  /* 初始化消息队列 */

int DoSendMsg(int msgid, struct msgbuf *msg, int length, int waittime);
/*
 * 从消息队列接收数据  
 * 参数∶ msgid    -- 消息队列句柄
 *        msg      -- 接收数据的缓冲区
 *        length   -- 缓冲区长度
 *        waittime -- 等待时间( <0∶永远等待, 0: 如不能接收立即返回 )
 * 返回∶ 0 -- 成功
 *       -1 -- 失败
 */

int DoRecvMsg(int msgid, struct msgbuf *msg, int length, int waittime);

int ShutUpSem(key_t SemKey);  /* 启动信号灯 */
int ShutDownSem(key_t SemKey); /* 停止信号灯 */
