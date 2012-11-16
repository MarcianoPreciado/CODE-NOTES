#include <iostream>
#include <vector>
#include "find2nd.h"

// 如果失败，返回0值。
template <typename T, typename compFunc>
const findSecondNode<T> *createTree(const findSecondNode<T> *array, int &size,
compFunc func)
{
  if (size <= 0) {
    size = 0;
    return static_cast<findSecondNode<T> *>(0);
    }
  if (size == 1){
    size = 0;
    return array;
    }

  bool isEvenSize;
  int rowSize;
  if ((size % 2) != 0) {
    isEvenSize = false;
    rowSize = size / 2 + 1;
  } else {
    isEvenSize = true;
    rowSize = size / 2;
  }

  findSecondNode<T> *row = new findSecondNode<T>[rowSize];
  int i;
  for (i = 0; i < size -1; i += 2)
    if (func(*(array[i].element), *(array[i + 1].element))) {
      row[i / 2] = findSecondNode<T>(array[i].element, array[i + 1].element, array + i);
    } else {
      row[i / 2] = findSecondNode<T>(array[i + 1].element, array[i].element, array
+ i + 1);
    }

  if (!isEvenSize) {
    row[rowSize - 1] = findSecondNode<T>(array[i].element,
static_cast<T *>(0), array +
i - 1);
  }
  size = rowSize;
  return row;
}

template <typename T, typename compFunc>
const T *findSecond( T *array, int size, compFunc func)
{
  if (size <= 1)
    return 0;
  if (size == 2)
    return func(array[0], array[1]) ? array : array + 1;
  findSecondNode<T> *row;

  bool evenNumSize;
  int rowSize;
  if ((size % 2) != 0) {
    evenNumSize = false;
    rowSize = size / 2 + 1;
  } else {
    evenNumSize = true;
    rowSize = size / 2;
  }

  row = new findSecondNode<T>[rowSize];

  int i;
  for (i = 0; i < size -1; i += 2)
    if (func(array[i], array[i + 1])) {
      row[i / 2] = findSecondNode<T>(array + i, array +i + 1,
static_cast<findSecondNode<T> *>(0));
    } else {
      row[i / 2] = findSecondNode<T>(array + i + 1, array + i,
static_cast<findSecondNode<T> *>(0));
    }

  if (!evenNumSize)
    row[rowSize -1] = findSecondNode<T>(array + i, static_cast<T *>(0),
static_cast<findSecondNode<T> *>(0));

  std::vector<const findSecondNode<T> *> destructor;
  destructor.push_back(row);
  const findSecondNode<T> *ptr = row;

  while ((ptr = createTree(ptr, rowSize, func)) !=
         static_cast<const findSecondNode<T> *>(0))
      destructor.push_back(ptr);
  destructor.pop_back();
  ptr = destructor.at(destructor.size() - 1);
  const T *maySecond = ptr->compareElement;
  const T *tmp;
  while (ptr->child) {
    tmp = ptr->child->compareElement;
    if (tmp != 0 && func(*tmp, *maySecond))
      maySecond = tmp;
    ptr = ptr->child;
  }
  for (int i = destructor.size() - 1; i >= 0; i--)
    delete [] destructor.at(i);
  return maySecond;
}

//最好不要比较double值，不精确，此处只是展示
bool compDouble(const double &a, const double &b)
{
  return a < b;
}
int main ( int argc, char *argv[] )
{

  int a[10] = {90, 234, 234, 11, 5435, 89, 2423, 122, 99, 99};

  const int *tmp = findSecond(a, 10, compInt);
  std::cout << *tmp << std::endl;

  double b[9] = {23.33, 2342.22, 12.33, 2134.222, 5555.33, 242.2, 888,
                 1.333, 2342.222};

//最好不要比较double值，不精确，此处只是展示
  const double *tmpDouble = findSecond(b, 9, compDouble);
  std::cout << *tmpDouble << std::endl;
  return 0;
}       /* ----------  end of function main  ---------- */
