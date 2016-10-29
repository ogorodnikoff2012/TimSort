#ifndef __TIMSORT_SLOWSORT_H__
#define __TIMSORT_SLOWSORT_H__

#include <iterator>
#include "utils.h"
#include "itertools.h"

namespace timsort {
    template <class RAIterator, class T = typename std::iterator_traits<RAIterator>::value_type,
        class Comparator = std::less<T>>
    void selectionSort(RAIterator begin, RAIterator end, Comparator cmp = Comparator()) {
        using std::swap;
        RAIterator unsortedBegin = begin;
        while (unsortedBegin != end) {
            RAIterator minElement = unsortedBegin;
            for (RAIterator iter = unsortedBegin + 1; iter != end; ++iter) {
                if (cmp(*iter, *minElement)) {
                    minElement = iter;
                }
            }
            swap(*unsortedBegin++, *minElement);
        }
    }

    template <class RAIterator, class T = typename std::iterator_traits<RAIterator>::value_type,
        class Comparator = std::less<T>>
    void insertionSort(RAIterator begin, RAIterator end, Comparator cmp = Comparator()) {
        using std::swap;
        for (RAIterator i = begin; i != end; ++i) {
            RAIterator j = i;
            while (j != begin && cmp(*j, *(j - 1))) {
                swap(*j, *(j - 1));
                --j;
            }
        }
    }

}

#endif // __TIMSORT_SLOWSORT_H__
