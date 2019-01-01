#ifndef _sorts_h__
#define _sorts_h__

#include <iostream>
#include <utility>
#include <iomanip>
#include <algorithm>
#include <memory>

using namespace std;
 
/*
 *  Bubble Sort
 *
 */

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

/*
 * Merge Sort
 * ==========
 * In all functions below, Iterator is a random access iterator supporting an index operator and pointer-like subtraction and addition.
 * Compartor is a function object that determines "less than".
 */

template<typename Iterator, typename Comparator> static void merge(Iterator first, const Iterator mid,
        const Iterator last,
        const Iterator buffer_start,
        Comparator Compare) noexcept;

template<typename Iterator, typename Comparator> void merge_sort(Iterator first, Iterator last,
                                                                  Iterator buffer, Comparator Compare) noexcept;

template<typename T, typename Iterator, typename Comparator> void merge_sort(Iterator first, Iterator last, Comparator Compare) noexcept
{
   // allocate a working buffer for our merges
   std::unique_ptr<T[]> work_buffer { std::make_unique<T[]>(last + 1 - first) };
    
   merge_sort(first, last, work_buffer.get(), Compare);
}

template<typename Iterator, typename Comparator> void merge_sort(Iterator first, Iterator last,
                                                                 const Iterator buffer, Comparator compare)  noexcept
{
  // Base case: the range [first, last] can no longer be subdivided; it is of length one.
  if (first < last) {

      /*
       * 1. Divide data structure in a left, first half and second, right half.
       */ 
      
      Iterator mid = first + (last - first) / 2; // Note: division binds first before first is added.
      
      /*
       * Recurse on the left half.
       */
      merge_sort(first, mid, buffer, compare);    

      /*
       * When left half recursion ends, recurse on right half of [first, last], which is [mid + 1, last]. 
       * Note: Both the left half and right half recursive descents implictly save the indecies of [first, mid] and [mid+1, last] on the stack.
       */
      merge_sort(mid + 1, last, buffer, compare);

      /*
       * 2. When recursion ends, merge the two sub arrays [first, mid] and [mid+1, last] into a sorted array in [first, last]
       */ 
      merge(first, mid, last, buffer, compare); // merge-and-sort step
  }
}

/*
 * Merges subarrays  [first, mid] and [mid + 1, last] into a sorted array in working buffer, buffer_start. Then copies
 * the working buffer over the original segement [first, last]
 */

template<typename Iterator, typename Comparator> static void merge(Iterator first, const Iterator mid, const Iterator last,
                                                                  const Iterator buffer_start, Comparator compare) noexcept
{
    Iterator first1 = first;
    Iterator last1 = mid;
    
    Iterator first2 = mid + 1;
    Iterator last2 = last;
        
    int index = 0;
    
    /* 
     * While both sub-arrays are not empty, copy the smaller item into the 
     * temporary array buffer.
     */
    Iterator buffer_cursor = buffer_start;
    
    for (; first1 <= last1 && first2 <= last2; ++buffer_cursor) {
        
        if ( compare(*first1, *first2) ) {
            
            *buffer_cursor = std::move(*first1++);

        } else {
            
            *buffer_cursor = std::move(*first2++);
        }
    }
    
    // finish off the first sub-array, if necessary
    for (;first1 <= last1; ++first1, ++buffer_cursor) {
        
        *buffer_cursor = std::move(*first1);
    }
    
    // finish off the second sub-array, if necessary
    for (;first2 <= last2; ++first2, ++buffer_cursor) {
        
        *buffer_cursor = std::move(*first2);
    }
         
   // Copy the temp array to the original array
   int length = last + 1 - first;

   Iterator start = buffer_start;
   
   for (Iterator end = start + length; start != end;) {
        
        *first++ = std::move(*start++);
   }
}

/*
 * Iterative version of Merge Sort 
 * ===============================
 *
 * Code below is a convert C++11 version of this java code:
 * http://www.sinbadsoft.com/blog/a-recursive-and-iterative-merge-sort-implementations/
*/
// Fwd reference
template<typename T, typename Iterator, typename Comparator > static void iter_merge(Iterator first, int start, int middle, int end, Comparator comparer,
                              T *work_buffer) noexcept; 

template<typename T, typename Iterator, typename Comparator> Iterator iter_merge_sort(Iterator first, Iterator last, Comparator comparer) noexcept
{
    auto length = last + 1 - first;

    std::unique_ptr<T[]> work_buffer { new T[length] };

    /*
     * Traverse array input from beginning to end, sorting adjacent subarrays from the bottom up. Subarrays are always a power of 2
     * in size, starting  size one (2 to the zero), then 2 (2 to the first), 4 (2 to the second) and so on. The number of iterations is:
     * log base 2(length) rounded up. 
     */
    for (int width = 1; width <= length / 2 + 1; width *= 2) {
        
        /*
         * merge adjacent subarrays of size width
         */  

        for (int start = width; start < length; start += 2 * width)  { // (2 * width) == sum of lengths of both subarrays.

            iter_merge(first, start - width, start, std::min<decltype(start)>(start + width, length), comparer, work_buffer.get()); 
        }
    }
    
    return first;
}

template<typename T, typename Iterator, typename Comparator > static void iter_merge(Iterator input, int start, int middle, int end,
                                                                                     Comparator comparer, T *work_buffer) noexcept
{
    auto length = end - start;

    auto left = 0, right = 0, current = 0;

    while (left < middle - start && right < end - middle)     {
         
        if ( comparer(input[start + left], input[middle + right]) ) {

           work_buffer[current++] = std::move(input[start + left++]);

        } else {  

            work_buffer[current++] = std::move(input[middle + right++]);
        }
    }
 
    while (right < end - middle) {

         work_buffer[current++] = std::move(input[middle + right++]);
    }
 
    while (left < middle - start) {

         work_buffer[current++] = std::move(input[start + left++]);
    }
 
    std::move(work_buffer, work_buffer + length, input + start); // move to start
}
#endif
