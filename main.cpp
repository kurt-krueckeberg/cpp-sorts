#include <memory>
#include <array>
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>
#include "sorts.h"
using namespace std;

int main() 
{
   vector<int> b(13);
  
   generate(b.begin(), b.end(), [n = 20]() mutable { return n--; });
   
   cout << "\nb[] = ";      
   
   copy(b.begin(), b.end(), ostream_iterator<int>(cout, ", "));
   
   cout << endl;
      
   bubblesort(b);
        
   cout << "bubblesort(b) = ";
   
   cout << flush;
   
   copy(b.begin(), b.end(), ostream_iterator<int>(cout, ", "));
   
   cout << endl;
   vector<int> v(9);
   
   generate(v.begin(), v.end(), [n = 0] () mutable { return ++n;} );
   
   copy(v.begin(),v.end(), ostream_iterator<int>(cout, ", "));
   
   cout << endl;
   bubblesort(v);
   
   copy(v.begin(),v.end(), ostream_iterator<int>(cout, ", "));
   
   cout << endl;
   return 0; 
}
 
