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
Heap::Heap() : BUFSIZE(10)
{
  int i;
  srand((unsigned int)(time(NULL)));
  for (i = 0; i < BUFSIZE; i++ ) {
    array.push_back(rand()% 1000);
  }
  buildMaxHeapify();
}  /* -----  end of method Heap::Heap  (constructor)  ----- */


void Heap::preMaxHeapify (int pos, int size )
{
  int l = left(pos);
  int r = right(pos);

  int largest;
  if (l < size && array[l] > array[pos])
    largest = l;
  else
    largest = pos;

  if (r < size && array[r] > array[largest])
    largest = r;

  if (largest != pos) {
    int tmp = array[largest];
    array[largest] = array[pos];
    array[pos] = tmp;
    preMaxHeapify(largest, size);
  }

  return ;
}		/* -----  end of method Heap::preMaxHeapify  ----- */

void Heap::maxHeapify(int pos)
{
  preMaxHeapify(pos, heapSize());
}

void Heap::buildMaxHeapify()
{
// size /2 - 1为堆的最后一个非叶子结点

  if (heapSize() <= 2)
    maxHeapify(0);
  for (int i = heapSize() / 2 - 1; i >= 0; i--) {
    maxHeapify(i);
  }
  return ;
}		// -----  end of method Heap::buildMaxHeapify  ----- 


void Heap::heapSort()
{
  buildMaxHeapify();

  for (int i = heapSize() - 1; i > 0; i--) {
    int tmp = array[i];
    array[i] = array[0];
    array[0] = tmp;
    preMaxHeapify(0, i);
  }
}

void Heap::maxHeapifyDelete ( int pos )
{
  if (pos >= heapSize())
    return;
  int tmp = array[heapSize() -1];
  array[pos] = tmp;
  array.pop_back();

  maxHeapify(pos);
}		/* -----  end of method Heap::delete  ----- */

int Heap::extractMax()
{
  int max = array[0];
  maxHeapifyDelete(0);
  return max;
}

void Heap::insert(int value)
{
  array.push_back(value);
  upShift(heapSize() - 1);
}

void Heap::upShift(int pos)
{
  if (pos <= 0 || pos >= heapSize())
    return;
  while (pos > 0 && array[parent(pos)] < array[pos]) {
    int tmp = array[parent(pos)];
    array[parent(pos)] = array[pos];
    array[pos] = tmp;
    pos = parent(pos);
  }
}

void Heap::increaseKey(int pos, int value)
{
  if (value < array[pos])
    return;
  array[pos] = value;
  upShift(pos);
}

