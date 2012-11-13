
#ifndef  heap_INC
#define  heap_INC
#include <vector>
#include <iostream>

using namespace std;

/*
 * =====================================================================================
 *        Class:  Heap
 *  Description:  
 * =====================================================================================
 */

class Heap
{
 public:
  /* ====================  LIFECYCLE     ======================================= */
  Heap();                             /* constructor */

  /* ======= =============  ACCESSORS     ======================================= */
  int parent(int pos) {
    if (pos == 0)
      return 0;
    return (pos -1) / 2;
  }

  int left(int pos) {
    return pos * 2 + 1;
  }

  int right(int pos) {
    return (pos + 1) * 2;
  }

  int heapSize(void) {
    return array.size();
  }

  void printAll() {
    for (int i = 0; i < heapSize(); i++)
      cout << array[i] << "\t";
  }

  /* ====================  MUTATORS      ======================================= */
  void buildMaxHeapify();
  void heapSort();
  void maxHeapifyDelete(int pos);
  int extractMax();
  void insert(int value);
  void increaseKey(int pos, int value);

  /* ====================  OPERATORS     ======================================= */

 protected:
  /* ====================  METHODS       ======================================= */

  /* ====================  DATA MEMBERS  ======================================= */

 private:
  /* ====================  METHODS       ======================================= */
  void maxHeapify(int pos);
  void preMaxHeapify(int pos, int size);
  void upShift(int pos);

  /* ====================  DATA MEMBERS  ======================================= */
  vector<int> array;
  const int BUFSIZE;

}; /* -----  end of class Heap  ----- */

#endif   /* ----- #ifndef heap_INC  ----- */
