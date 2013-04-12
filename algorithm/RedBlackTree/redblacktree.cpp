#include "redblacktree.h"
#include <cstdlib>
#include <ctime>

template <typename Comparable>
typename RedBlackTree<Comparable>::RedBlackNode *RedBlackTree<Comparable>::findMinPos(RedBlackNode *pos) const
{
  if (pos == nullNode)
    return pos;
  while (pos->left != nullNode)
    pos = pos->left;
  return pos;
}

template <typename Comparable>
typename RedBlackTree<Comparable>::RedBlackNode
*RedBlackTree<Comparable>::findMaxPos(RedBlackNode *pos) const
{
  if (pos == NULL)
    return pos;
  // 此处用的是nullNode,而非NULL,也是红黑树与普通二叉树的NULL区别.
  while (pos->left != nullNode)
    pos = pos->left;
  return pos;
}

template <typename Comparable>
typename RedBlackTree<Comparable>::RedBlackNode
*RedBlackTree<Comparable>::find(
    RedBlackTree<Comparable>::RedBlackNode *pos, const Comparable &element) const
{
  if (pos == NULL)
    return pos;
  while (pos != nullNode && pos->element != element) {
      if (pos->element < element)
        pos = pos->right;
      else if (pos ->element > element)
        pos = pos->left;
    }
  return pos;
}

template <typename Comparable>
typename RedBlackTree<Comparable>::RedBlackNode
*RedBlackTree<Comparable>::findSuccessor(RedBlackTree::RedBlackNode *pos)
{
  if (pos->right != nullNode)
    return findMinPos(pos->right);
  RedBlackNode *successor = pos->parent;
  while (successor != nullNode && pos == successor->right) {
      pos = successor;
      successor = pos->parent;
    }
  return successor;
}

template <typename Comparable>
bool RedBlackTree<Comparable>::isEmpty() const
{
  return treeHeader() == nullNode;
}

template <typename Comparable>
void RedBlackTree<Comparable>::printTree(RedBlackNode *pos) const
{
  if (pos == NULL || pos == nullNode)
    return;
  if (pos->left != nullNode)
    printTree(pos->left);
  //WARNING: 此处cout只适用于可以打印的元素,请参照自己的数据结构实现
  std::cout << pos->element;
  if (pos->color == BLACK)
    std::cout << ":BLACK";
  else
    std::cout << ":RED";
  std::cout << "\t";
  if (pos->right != nullNode)
    printTree(pos->right);
}

template <typename Comparable>
void RedBlackTree<Comparable>::makeEmpty()
{
  emptySubTree(header);
  header = nullNode;
}

template <typename Comparable>
void RedBlackTree<Comparable>::emptySubTree(RedBlackNode *pos) {
  if (pos == nullNode)
    return;
  emptySubTree(pos->left);
  emptySubTree(pos->right);
  delete pos;
}

template <typename Comparable>
void RedBlackTree<Comparable>::rbInsert(const Comparable &x)
{
  RedBlackNode *preFindPos = nullNode;
  RedBlackNode *findPos = header;

  while (findPos != nullNode) {
      preFindPos = findPos;
      if (findPos->element > x)
        findPos = findPos->left;
      else
        findPos = findPos->right;
    }

  // 新节点元素为x，默认为红色，父节点为preFindPos，其他为nullNode。
  RedBlackNode *newNode = new RedBlackNode(x, preFindPos, nullNode, nullNode, RED);
  newNode->element = x;
  newNode->parent = preFindPos;
  if (preFindPos == nullNode)
    header = newNode;
  else if (x < preFindPos->element)
    preFindPos->left = newNode;
  else
    preFindPos->right = newNode;

  rbInsertFixup(newNode);
}

template <typename Comparable>
void RedBlackTree<Comparable>::rbInsertFixup(RedBlackTree::RedBlackNode *node)
{
  RedBlackNode *uncle;
  while (node->parent->color == RED) {
      //以下六种情况前提均是，父节点为红，爷结点为黑

      // 如果新插入结点node，执行以下操作：
      // 因为根节点为黑色，根节点的子节点不执行此条语句，所以node的爷爷结点肯定不是nullNode。
      // 访问不会越界。
      if (node->parent == node->parent->parent->left) {
          uncle = node->parent->parent->right;
          if (uncle->color == RED) { // 第一种情况：node与其父结点，叔父结点均为RED
              // 此时违反性质4：红节点的两个子节点均为黑，于是：
              // 将 父、叔结点改为黑，
              // 爷爷结点改为红，以维持性质五：黑高度相同。
              node->parent->color = BLACK;
              uncle->color = BLACK;
              node->parent->parent->color = RED;
              // node上移两层，迭代考察前爷爷结点为红是否违反性质4
              node = node->parent->parent;
            } else  {  // 叔父结点为黑色，父节点为红的情况。
              if (node == node->parent->right) {
                  //第二种情况：node父节点为左节点，node为右结点
                  node = node->parent;
                  // 左旋，将情况改为第三种情况
                  leftRotate(node);
                }
              // 第三种情况：node父节点为左节点，node为左节点。
              node->parent->color = BLACK; //uncle为黑，node为左红节点时的情况
              node->parent->parent->color = RED;
              rightRotate(node->parent->parent);
            }
        } else {
          //else 语句执行 node的父节点为右节点时的情况。
          uncle = node->parent->parent->left;
          if (uncle->color == RED) { // 第四种情况：父节点为右结点，node与其父，叔父结点均为RED
              // 此时违反性质4：红节点的两个子节点均为黑，于是：
              // 将 父、叔结点改为黑，
              // 爷爷结点改为红，以维持性质五：黑高度相同。
              node->parent->color = BLACK;
              uncle->color = BLACK;
              node->parent->parent->color = RED;
              // node上移两层，迭代考察前爷爷结点为红是否违反性质4
              node = node->parent->parent;
            } else  {  // 父节点为红，叔结点为黑色的情况。
              if (node == node->parent->left) {
                  // 第五种情况 父节点为右结点，node为左节点，叔结点为黑。
                  node = node->parent;
                  // 右旋，将情况改为
                  rightRotate(node);
                }
              // 第六种情况 父节点为右结点，node为右节点，叔结点为黑。
              node->parent->color = BLACK; //uncle为黑，node为左红节点时的情况
              node->parent->parent->color = RED;
              leftRotate(node->parent->parent);
            }
        }
    }
  header->color = BLACK;
}

template <typename Comparable>
void RedBlackTree<Comparable>::rbDelete(const Comparable &x)
{
  RedBlackNode *ptr = find(header, x);
  if (ptr != nullNode){
    RedBlackNode *delNode = rbDelete(ptr);
    if (delNode != nullNode)
      delete delNode;
  }
}

template <typename Comparable>
typename RedBlackTree<Comparable>::RedBlackNode *
RedBlackTree<Comparable>::rbDelete(RedBlackTree::RedBlackNode *node)
{
  RedBlackNode *delNode;
  if (node->left == nullNode || node->right == nullNode)
    delNode = node;
  else
    delNode = findSuccessor(node);

  RedBlackNode *delNodeChild;
  // 以下if...else...语句要结合上边来看
  // 如果node左右子女有一个为nullNode，则下面语句找到要提升的delNodeChild。
  // 如果某节点有两个子女，则其后继没有左子女。此时如果delNode = findSuccessor的话，
  // delNode->left一定等于nullNode，肯定会执行delNodeChild = delNode->right。

  if (delNode->left != nullNode)
    delNodeChild = delNode->left;
  else
    delNodeChild = delNode->right;

  delNodeChild->parent = delNode->parent;
  if (delNode->parent == nullNode)
    header = delNodeChild;
  else if (delNode == delNode->parent->left)
    delNode->parent->left = delNodeChild;
  else
    delNode->parent->right = delNodeChild;

  if (delNode != node)
    node->element = delNode->element;

  // 如果delNode->color为红的话，则红黑性质得以保持。
  // 因为此时delNode肯定不为根，根节点仍为黑
  // delNode的父节点肯定为黑，被提升的delNodeChild不会与之违反性质：红节点的子节点不能有红
  // 黑高度没有变化

  if (delNode->color == BLACK)
    rbDeleteFixup(delNodeChild);

  // WARNNING:此处没有delete delNode，用户需要接收此函数返回值然后delete
  // 或者在此处delete，且将函数返回类型设置为void。
  return delNode;
}

template <typename Comparable>
void RedBlackTree<Comparable>::rbDeleteFixup(RedBlackTree::RedBlackNode *node)
{
  // 此时node->color因为父亲黑节点的删除，将其视为具有双重颜色特性，为黑黑或者红黑，要调整
  while (node != header && node->color == BLACK) {
      // while循环处理node->color为黑且不为header的情况,此时node为黑黑的双重黑特性。
      if (node == node->parent->left)
        leftChildDeleteFixup(node);
      else
        rightChildDeleteFixup(node);
    }

  // 如果node->color为红或者为header的话，此时只需简单的将其设置为BLACK即可，见此:
  node->color =BLACK;
}

template <typename Comparable>
void RedBlackTree<Comparable>::leftChildDeleteFixup(RedBlackTree::RedBlackNode * &node)
{
  //node为双重黑特性。
  RedBlackNode *rightNode = node->parent->right;
  // case 1: rightNode为红，两个子节点为黑,将其转换为以下case
  if (rightNode->color == RED) {
      rightNode->color = BLACK;
      node->parent->color = RED;
      leftRotate(node->parent);
      // node的右子节点为旋转之前rightNode的左黑孩子，继续进入下面的case以维持红黑树性质
      rightNode = node->parent->right;
    }

  // case 2: rightNode与其两个子节点均为黑。
  if (rightNode->left->color == BLACK && rightNode->right->color == BLACK) {
      rightNode->color = RED;
      //简单将rightNode改为黑即可维持黑高度特性，node的双重特性取消，为单色。
      // 此时将node设为其父，考察其父的红黑性质。
      node = node->parent;
    } else {
      // case 3:rightNode与其右孩子为黑，左孩子为红，将其旋转后进入case 4
      if (rightNode->right->color== BLACK) {
          rightNode->left->color = BLACK;
          rightNode->color = RED;
          rightRotate(rightNode);
          rightNode = node->parent->right;
        }
      // case 4:rightnode为黑，其右孩子为红

      //rightNode颜色为其父颜色，旋转后以维持原来的黑高度
      rightNode->color = node->parent->color;

      // TODO:此时意义不明。。。node->parent此时一定为红色吗？
      node->parent->color= BLACK;
      rightNode->right->color = BLACK;
      leftRotate(node->parent);
      node = header;
    }
}

template <typename Comparable>
void RedBlackTree<Comparable>::rightChildDeleteFixup(RedBlackTree::RedBlackNode *&node)
{
  //node为双重黑特性。
  RedBlackNode *leftNode = node->parent->left;
  // case 1: rightNode为红，两个子节点为黑,将其转换为以下case
  if (leftNode->color == RED) {
      leftNode->color = BLACK;
      node->parent->color = RED;
      rightRotate(node->parent);
      // node的左子节点为旋转之前leftNode的右黑孩子，继续进入下面的case以维持红黑树性质
      leftNode = node->parent->left;
    }

  // case 2: lefttNode与其两个子节点均为黑。
  if (leftNode->left->color == BLACK && leftNode->right->color == BLACK) {
      leftNode->color = RED;
      //简单将lefttNode改为黑即可维持黑高度特性，node的双重特性取消，为单色。
      // 此时将node设为其父，考察其父的红黑性质。
      node = node->parent;
    } else {
      // case 3:leftNode与其左孩子为黑，右孩子为红，将其旋转后进入case 4
      if (leftNode->left->color== BLACK) {
          leftNode->right->color = BLACK;
          leftNode->color = RED;
          leftRotate(leftNode);
          leftNode = node->parent->left;
        }
      // case 4:leftNode为黑，其左孩子为红

      //leftNode颜色为其父颜色，旋转后以维持原来的黑高度
      leftNode->color = node->parent->color;

      // TODO:此时意义不明。。。node->parent此时一定为红色吗？
      node->parent->color= BLACK;
      leftNode->left->color = BLACK;
      rightRotate(node->parent);
      node = header;
    }
}

// 当在结点X上做左旋时，我们假设他的右孩子不是nullNode，
// x可以为任意右孩子不是nullNode的结点。

//左旋时，所影响到的结点，只有x，x的右孩子，与x右孩子的左节点。
template <typename Comparable>
void RedBlackTree<Comparable>::leftRotate(RedBlackTree::RedBlackNode *x)
{
  RedBlackNode *y = x->right;
  x->right = y->left;  // 旋转中，x的右结点设为y的左结点
  if (y->left != nullNode)
    y->left->parent = x;
  y->parent= x->parent;
  if (x->parent == nullNode) {
      header = y;
    } else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

//左旋时，所影响到的结点，只有x，x的左孩子，与x左孩子的右节点。
template <typename Comparable>
void RedBlackTree<Comparable>::rightRotate(RedBlackTree::RedBlackNode *x)
{
  RedBlackNode *y = x->left;
  x->left = y->right;
  if (y->right != nullNode)
    y->right->parent = x;
  y->parent = x->parent;
  if (x->parent == nullNode)
    header = y;
  else if ( x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->right = x;
  x->parent = y;
}

int main(int argc, char *argv[])
{
  RedBlackTree<int> tmp;
  srand(unsigned(time(NULL)));
  for (int i = 0; i < 20; i++)
    tmp.rbInsert(rand() % 10000);

  tmp.printTree();
  std::cout << std::endl;
  std::cout << std::endl;

  tmp.makeEmpty();
  tmp.rbInsert(12);
  tmp.rbInsert(1);
  tmp.rbInsert(9);
  tmp.rbInsert(2);
  tmp.rbInsert(0);
  tmp.rbInsert(11);
  tmp.rbInsert(7);
  tmp.rbInsert(19);
  tmp.rbInsert(4);
  tmp.rbInsert(15);
  tmp.rbInsert(18);
  tmp.rbInsert(5);
  tmp.rbInsert(14);
  tmp.rbInsert(13);
  tmp.rbInsert(10);
  tmp.rbInsert(16);
  tmp.rbInsert(6);
  tmp.rbInsert(3);
  tmp.rbInsert(8);
  tmp.rbInsert(17);
  tmp.printTree();

  std::cout << std::endl << std::endl;
  tmp.rbDelete(12);
  tmp.rbDelete(1);
  tmp.rbDelete(9);
  tmp.rbDelete(2);
  tmp.rbDelete(0);
  tmp.printTree();

  return 0;
}
