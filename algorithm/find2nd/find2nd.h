#include <iostream>
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  findSecond
 *  Description:  算法导论练习 9.1-1
 *  证明：在最坏情况下，利用n + lgn -2次比较，，即可找到n个元素中的第2个小元素
 *  （提示：同时找最小元素）
 *  限制：需要支持随机迭代
 *  思路：起先想到用分治算法，但设计不出，还是忍不住看了算法导论的答案：
 *
 * To find the smallest element construct a tournament as follows:
 * Compare all the numbers in pairs.
 * Only the smallest number of each pair is potentially the
 smallest of all so the problem is reduced to size n/2 .
 * Continuing in this fashion until there is only one left clearly solves the problem.
 * Exactly n − 1 comparisons are needed since the tournament can be drawn as an n-leaf binary
tree
  which has n − 1 internal nodes (show by induction on n).
 * Each of these nodes correspond to a comparison.
 * We can use this binary tree to also locate the second
 * smallest number. The path from the root to the smallest element
 * (of height lg n ) must contain the second smallest element. Conducting
 * a tournament among these uses lg n − 1 comparisons.
 * The total number of comparisons are: n − 1 + lg n − 1 = n + lg n − 2.
 * =====================================================================================
 */

bool compInt(const int &a, const int &b)
{
  return a < b;
}

template <typename T>
class findSecondNode {
public:
  findSecondNode() : element(0), compareElement(0), child(0) { }
  findSecondNode(const T *elementArg, const T *compareElementArg, const findSecondNode<T> *childArg)
    : element(elementArg), compareElement(compareElementArg), child(childArg) { }
  const T *element;          /* 本元素指针 */
  const T *compareElement;   /* 与本元素比较的元素指针 */
  const findSecondNode<T> *child;            /* 二叉树子节点，与本元素同值 */
};

template <typename T, typename compFunc>
const findSecondNode<T> *createTree(const findSecondNode<T> *array, int &size, compFunc func);

template <typename T, typename compFunc>
const T *findSecond(T *array, int size, compFunc func);

