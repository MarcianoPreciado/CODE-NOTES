#include "heap.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Heap
 *      Method:  Heap
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Heap::Heap() : BUFSIZE(500)
{
  uint i;
  for (i = 0; i < BUFSIZE; i++ ) {
    srand(time(NULL));
    array.push_back(rand());
  }
}  /* -----  end of method Heap::Heap  (constructor)  ----- */


void
Heap::maxHeapify(unsigned int pos)
{
  uint l = left(pos);
  uint r = right(pos);

  uint largest;
  if (l < heapSize() && array[l] > array[pos])
    largest = l;
  else
    largest = pos;

  if (r < heapSize() && array[r] > array[largest])
    largest = r;

  if (largest != pos) {
    int tmp = array[largest];
    array[largest] = array[pos];
    array[pos] = tmp;
    maxHeapify(largest);
  }
}

void Heap::buildMaxHeapify()
{
// size -1 /2 为堆的最后一个非叶子结点
  for ( uint i = (heapSize() - 1) / 2 ; i > 0; i--) {
    maxHeapify(i);
  }
  return ;
}		// -----  end of method Heap::buildMaxHeapify  ----- 

