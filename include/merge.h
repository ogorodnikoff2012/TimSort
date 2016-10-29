#ifndef __TIMSORT_MERGE_H__
#define __TIMSORT_MERGE_H__

#include "utils.h"
#include "itertools.h"
#include "params.h"
#include "adaptiveLowerBound.h"

namespace timsort {
    template <class RAIterator, class Comparator>
    void mergeWithBuffer(RAIterator begin, RAIterator border, RAIterator end, 
            RAIterator bufBegin, Comparator cmp, const ITimSortParams &params) {
        using std::swap;
        std::size_t leftLength = std::distance(begin, border);
        swapBlocks(begin, bufBegin, leftLength);
        RAIterator bufEnd = bufBegin + leftLength;
        RAIterator it1 = bufBegin, it2 = border, itAns = begin;

        bool previousFromFirst = true;
        std::size_t count = 0;
        std::uint32_t gallop = params.getGallop();

        while (it1 != bufEnd && it2 != end) {
            bool takeFromFirst = !cmp(*it2, *it1);
            if (takeFromFirst != previousFromFirst) {
                count = 0;
            }

            if (count >= gallop) {
                RAIterator it = adaptiveLowerBound(takeFromFirst ? it1 : it2,
                        takeFromFirst ? bufEnd : end, takeFromFirst ? *it2 : *it1, cmp);
                RAIterator &itToBeGalloped = takeFromFirst ? it1 : it2;
                while (itToBeGalloped != it) {
                    swap(*itToBeGalloped++, *itAns++);
                }
                count = 0;
            } else {
                swap(*(takeFromFirst ? it1 : it2)++, *itAns++);
            }
            ++count;
            previousFromFirst = takeFromFirst;
        }

        while (it1 != bufEnd) {
            swap(*it1++, *itAns++);
        }   

        while (it2 != end) {
            swap(*it2++, *itAns++);
        }
    }

    template <class RAIterator, class Comparator,
             class T = typename std::iterator_traits<RAIterator>::value_type>
    void backwardMergeWithBuffer(RAIterator begin, RAIterator border, RAIterator end, 
            RAIterator bufBegin, Comparator cmp, const ITimSortParams &params) {
        std::size_t length = std::distance(border, end);
        mergeWithBuffer(reverseIter(end), reverseIter(border), reverseIter(begin),
                reverseIter(bufBegin + length), InvertedComparator<Comparator, T>(cmp), params);
    }


}

#endif // __TIMSORT_MERGE_H__
