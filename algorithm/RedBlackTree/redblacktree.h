/*
 * =====================================================================================
 *
 *       Filename:  redblacktree.h
 *
 *    Description:  红黑树的实现：
 *    红黑树确保没有一条路径会比其他路径长出两倍，接近平衡。
 *
 *    红黑树的5个基本性质：
 *    1,每个节点为红或黑
 *    2,根节点为黑
 *    3,每个叶结点(nullNode)为黑
 *    4,如果一个节点是红的，那他的两个儿子都是黑的
 *    5,对每个节点，从该结点到其子孙节点的所有路径上包含相同数目的黑节点
 *
 *        Version:  1.0
 *        Created:  2012年11月16日 17时47分03秒
 *         Author:  sd44 (sd44sd44@yeah.net), 
 *
 * =====================================================================================
 */

#ifndef  redblacktree_INC
#define  redblacktree_INC

#include <iostream>

template <typename Comparable>
class RedBlackTree
{
  // 调用以下返回RedBlackNode *类型的函数时，注意要检查值是不是nullNode，再进行操作。
 public:
  enum TreeColor {RED, BLACK};
  struct RedBlackNode {
    Comparable element;
    RedBlackNode *parent;
    RedBlackNode *left;
    RedBlackNode *right;
    TreeColor color;
    RedBlackNode( const Comparable & theElement = Comparable( ), RedBlackNode *par = NULL,
                  RedBlackNode *lt = NULL, RedBlackNode *rt = NULL,
                  TreeColor defaultColor = RED )
      : element( theElement ), parent(par), left( lt ), right( rt ), color(defaultColor)
    { }
  };

  // 显示构造函数，传入一个可以< >比较的元素
  explicit RedBlackTree() {
    nullNode = new RedBlackNode;
    // nullNode的初始化未必有必要吧？
    nullNode->parent = nullNode->left = nullNode->right = nullNode;
    nullNode->color = BLACK;
    header = nullNode;
  }

  ~RedBlackTree() {
    makeEmpty();
    delete nullNode;
  }

  const Comparable &findMin() {
    return findMinPos(header)->element;
  }
  const Comparable &findMax() {
    return findMaxPos(header)->element;
  }

  RedBlackNode *treeHeader() { return header; }
  RedBlackNode *treeNullNode() { return nullNode; }

  RedBlackNode *find(RedBlackNode *pos, const Comparable &element) const;
  RedBlackNode *findSuccessor(RedBlackNode *pos);
  bool isEmpty() const;
  void printTree() const { printTree(header); }
  void printTree(RedBlackNode *pos) const;
  void makeEmpty();
  void rbInsert(const Comparable &x);
  void rbDelete(const Comparable &x);
  RedBlackNode *rbDelete(RedBlackNode *node);

  void leftChildDeleteFixup(RedBlackTree::RedBlackNode * &node);
  void rightChildDeleteFixup(RedBlackTree::RedBlackNode * &node);
private:

  RedBlackNode *findMinPos(RedBlackNode *pos) const;
  RedBlackNode *findMaxPos(RedBlackNode *pos) const;
  void rbInsertFixup(RedBlackNode *node);
  void rbDeleteFixup(RedBlackNode *node);
  void leftRotate(RedBlackNode *x);
  void rightRotate(RedBlackNode *x);
  void emptySubTree(RedBlackNode *pos);

  RedBlackNode *header;
  RedBlackNode *nullNode;

};
#endif   /* ----- #ifndef redblacktree_INC  ----- */
