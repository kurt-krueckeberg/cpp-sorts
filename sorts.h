#ifndef _sorts_h__
#define _sorts_h__

#include <iostream>
#include <utility>
#include <iomanip>
#include <iostream>
using namespace std;
 
template<class T> void bubblesort(T& v)
{
  cout << "In bubblesort: array size = " << v.size() << ". Outer loop will execute " << v.size() - 1 << " times." << endl;
  auto cnt = 1;
  
  bool sorted = true; // optimization
  int size = v.size();
  
  // Out comparison loop. Loop size - 1 times
  for (int k = 0; k < size - 1; ++k) {

    // After this loop the largest element between [0, size - k - 1) resides at the end, v[size - k - 1]    
    for (auto i =  0; i < size - k - 1; ++i) {

        if (v[i] > v[i + 1]) {
            swap(v[i], v[i + 1]);  
            sorted = false;
        }    
    }
    
    if (sorted)
        break;
    
    cout << setw(2) << right << cnt++ << ".  Inner loop was done " << (v.size() - k - 1) << " times." << endl; 
  }
  
}


#endif
