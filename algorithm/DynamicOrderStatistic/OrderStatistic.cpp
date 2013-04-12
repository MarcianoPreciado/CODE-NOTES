/*
 * =====================================================================================
 *
 *       Filename:  OrderStatistic.cpp
 *
 *    Description:  算法导论 第14章 动态顺序统计树练习题解答（伪代码）
 *
 *        Version:  1.0
 *        Created:  2012年11月19日 22时59分16秒
 *         Author:  sd44
 *   Organization:  
 *
 * =====================================================================================
 */

// 14.1-3 OS-SELECT的非递归

OS-SELECT(ptr x, int i) {
  if (i > x->size)
    return NULL;
  int r;
  ptr tmp = x;
  while (1) {
    r = tmp->left->size + 1;
    if (i < r) {
      tmp = tmp->left;
    } else if (i > r) {
      i = i - r;
      tmp = tmp->right;
    }
    else
      return r;
  }
}

// 14.1-4 OS-KEY-RANK(T, k) 较为简单，稍微修改树的find(元素）操作即可，
// 题目要求为递归形式，我实现的是非递归形式。
// 解答：
//
 r为动态集合中k的秩。。

 r初始化为0.
 如果k比当前节点的元素值大，则向右子节点移动，并记录当前节点左子结点的size且
 +1。r += currentNode->left->size + 1;
 如果k比当前节点的元素值小，则向左子结点移动，不对r进行加减操作。
 如果k等于当前节点的元素时，r += currentNode->left->size + 1;


//14.1-5 如何在O(lg N)的时间内，确定x在树的线性序中第i个后继。
//解答：
//
首先，find找到x在顺序统计树中的节点node。
如果node右节点的size大于i，则进入右结点，node = node->right,递归找第i个后继

如果node右结点的size小于i，则i = i - node->right->size，此时分两种情况：
1, 如果node为其父的右结点，则上移，node = node->parent

2, 如果node为其父的左节点，则上移，node = node->parent，i = i - 1。如果i == 1,返回此时的node(为原来node的parent）。
 否则递归找第i个后继

