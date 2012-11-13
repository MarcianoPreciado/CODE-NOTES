#include "heap.h"

int main ( int argc, char *argv[] )
{
  Heap xxx;
  xxx.printAll();
  cout << "\n\n";
  xxx.heapSort();
  cout << "Now Starting Heap Sort" << endl;
  xxx.printAll();
  cout << endl;

  xxx.buildMaxHeapify();
  xxx.increaseKey(9, 2000);
  xxx.heapSort();
  cout << "Now Starting Heap Sort" << endl;
  xxx.printAll();
  cout << endl;

  return 0;
}				// ----------  end of function main  ---------- 
