#include <iostream>

int intFunc(int *number, void *)
{
  return *number;
}
// intbit 获取整数的第dbit位
int intbit(int *number, void *dbit);
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  CountingSort
 *  Description:  计数排序计数提供了O(N)的排序算法。
 *  但受限于整数的Range，是一个用空间换取时间的算法。
 *  rangeArr的依次相加获取数组位置是为了稳定排序，适用于各种可由正整数排序的结构。
 *  限制：必须是0开始的有range的正整数（可以通过func转换为此，再进行排序)
 * =====================================================================================
 */
template <typename T>
struct PosWithPointer {
  int posValue;
  T *pointer;
};

template <typename T>
void CountingSort(T *array, int size, int range, int(*func)(T *element, void *), void *arg = 0)
{
     PosWithPointer<T> *rangeArr = new PosWithPointer<T>[range +1];
     for (int i = 0; i <= range; i++) {
          rangeArr[i].posValue = 0;
          rangeArr[i].pointer = 0;
     }
     for (int i = 0;  i < size; i++) {
          ++rangeArr[func(array + i, arg)].posValue;
          rangeArr[func(array +i, arg)].pointer = array + i;
     }

     for (int i = 1; i <= range; i++) {
          rangeArr[i].posValue += rangeArr[i - 1].posValue;
     }

     T *sortArray = new T[size];
     for (int i = size - 1; i >= 0; --i) {
          // 注意此处要 -1,因为最小的数据位置为1,而数组初始为0.
          sortArray[rangeArr[func(array + i, arg)].posValue - 1] = array[i];
          --rangeArr[func(array +i, arg)].posValue;
     }
     for (int i = 0; i < size; i++) {
          array[i] = sortArray[i];
     }
     delete [] rangeArr;
     delete [] sortArray;

}

int tenPow(int basePow)
{
  int result = 1;
  for (int i = 1; i <= basePow; i++)
    result *= 10;
  return result;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  RadixSort
 *  Description:  借助于计数排序的RaDixSort;
 *  算法复杂度：线性 O(dbit(size+ 10)), 10为每一位可能的取值
 * =====================================================================================
 */
// Dgit为数字的最大位数
void RadixSort(int *array, int size, int dbit)
{
  for (int i = 1; i <= dbit; i++)
    CountingSort(array, size, tenPow(dbit), intbit, static_cast<void *>(&i));
}

int intbit(int *number, void *dbit)
{
  if (*number < 0)
    return 0;
 
  int *bit = static_cast<int *>(dbit);
  return *number / tenPow(*bit - 1) % 10;
}

int main(int argc, char **argv)
{
     int a[10] = {3, 5, 3, 2, 8, 984, 223, 82, 9234, 10};
     std::cout << "计数排序数组" << std::endl;
     CountingSort(a, 10, 9235, intFunc, 0);
     for (int i = 0; i < 10; i++) {
          std::cout << a[i] << "\t";
     }

     std::cout << "\n使用基数排序，底层用计数排序实现" << std::endl;
     int b[10] = {3, 5, 3, 2, 8, 984, 223, 82, 9234, 10};
     RadixSort(b, 10, 4);
     for (int i = 0; i < 10; i++) {
          std::cout << b[i] << "\t";
     }

     return 0;
}
