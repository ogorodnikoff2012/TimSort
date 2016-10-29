#ifndef __TIMSORT_MERGE_H__
#define __TIMSORT_MERGE_H__

#include "utils.h"
#include "itertools.h"

namespace timsort {
    template <class RAIterator, class Comparator>
    void mergeWithBuffer(RAIterator leftBegin, RAIterator leftEnd, 
            RAIterator rightBegin, RAIterator rightEnd, 
            RAIterator bufBegin, Comparator cmp) {
        std::size_t leftLength = std::distance(leftBegin, leftEnd);
        swapBlocks(leftBegin, bufBegin, leftLength);
        RAIterator bufEnd = bufBegin + leftLength;
        RAIterator it1 = bufBegin, it2 = rightBegin, itAns = leftBegin;
        while (it1 != bufEnd && it2 != rightEnd) {
            if (!cmp(*it2, *it1)) {
                std::swap(*it1++, *itAns++);
            } else {
                std::swap(*it2++, *itAns++);
            }
            if (itAns == leftEnd) {
                itAns = rightBegin;
            }
        }

        while (it1 != bufEnd) {
            std::swap(*it1++, *itAns++);
            if (itAns == leftEnd) {
                itAns = rightBegin;
            }
        }   

        while (it2 != rightEnd) {
            std::swap(*it2++, *itAns++);
            if (itAns == leftEnd) {
                itAns = rightBegin;
            }
        }
    }

    template <class RAIterator, class Comparator,
             class T = typename std::iterator_traits<RAIterator>::value_type>
    void backwardMergeWithBuffer(RAIterator leftBegin, RAIterator leftEnd, 
            RAIterator rightBegin, RAIterator rightEnd, 
            RAIterator bufBegin, Comparator cmp) {
        double length = std::distance(rightBegin, rightEnd);
        mergeWithBuffer(reverseIter(rightEnd), reverseIter(rightBegin),
                reverseIter(leftEnd), reverseIter(leftBegin),
                reverseIter(bufBegin + length), InvertedComparator<Comparator, T>(cmp));
    }


}

#endif // __TIMSORT_MERGE_H__
