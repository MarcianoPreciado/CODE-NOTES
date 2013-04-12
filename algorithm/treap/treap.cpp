struct treap {
   type element;
   int priority ;

}

treap array[n]; //由用户构建一个带有元素与互异优先级的数组。

for (int i = 0; i < n; i++) {
   treePtr = treapTreeInsert(array[i].elemnt)  //执行普通的二叉树插入,保存插入后的节点
   while (treePtr != treepHeader && treePtr->priority < treePtr->parent->priority) {
     // treePtr不为树根，且优先级比其父节点小。
      if (treePtr = treePtr->parent->left)
        rightRorate(treePtr->parent);
      else
        leftRorate(treePtr->parent);

         
