/**
 *       @file  soapList.h
 *      @brief  存储SOAP中XML DOM的元素名以及数据的链表实现。
 *
 * Detailed description starts here.
 *     因为是顺序链表结构，插入性能上性能为O（N），但考虑数据量较小以及形成一
 *     易懂的格式，仍然采用链表结构。
 *
 *     soapList所有节点指针，必须是calloc/malloc的。因为在soapList_isEmpty函数
 *   函数中，会对所有节点进行free。
 *
 *     @author  hoho sd44 (), sd44sd44@yeah.net
 *
 *   @internal
 *     Created  2013年03月19日
 *    Revision  $Id: doxygen.templates,v 1.3 2010/07/06 09:20:12 mehner Exp $
 *     Company
 *   Copyright  Copyright (c) 2013, hoho sd44
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License as published by the Free Software Foundation.
 * =====================================================================================
 */

#ifndef SOAPLIST_H
#define SOAPLIST_H


struct soapList
{
  char *elementName; /**< @brief XML元素名 */
  char *elementData; /**< @brief XML元素的数据 */
  struct soapList *parent; /** < @brief XML元素父节点内容在链表中存储的位置 ,
                               有助于在多重同名元素中判定是否所需元素*/
  struct soapList *next;
};

/**
 * @brief   判定soapList链表是否为空
 * @param   header soapList节点指针，一定要是calloc/malloc的
 * @return  如果链表为空，则返回1,否则为0
 */
int soapList_isEmpty(struct soapList *header);

/**
 * @brief   在soapList链表中插入相应节点
 * @param   header 在header位置之后插入
 * @param   parent XML元素父节点在链表中的指针
 * @param   name   XML元素名
 * @param   data   XML元素内容
 * @return  返回所插入节点的指针
 */
struct soapList *soapList_insert(struct soapList *header, struct soapList *parent,
                                 const char *name, const char *data);

/**
 * @brief   根据元素名查找所在节点
 * @param   name 必须是完全匹配输出格式，如 "parent:child:grandchild"
 * @return  返回所在节点或NULL指针。
 */
struct soapList *soapList_findByName(struct soapList *header, const char *name);

/**
 * @brief   析构free整个链表中malloc的数据
 */
void soapList_emptyList(struct soapList *header);


/**
 * @brief   打印整个链表数据，用以帮助用户理解结构以及调试程序输出。
 */
void soapList_printAll(struct soapList *header);

#endif // SOAPLIST_H
