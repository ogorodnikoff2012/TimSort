#ifndef TIMSORT_ADAPTIVE_LOWER_BOUND_H
#define TIMSORT_ADAPTIVE_LOWER_BOUND_H

#include <iterator>

namespace timsort {
    template <class RAIterator, class T = typename std::iterator_traits<RAIterator>::value_type, class Comparator>
    RAIterator lowerBound(RAIterator begin, RAIterator end, const T &val, Comparator cmp) {
        if (begin == end || cmp(val, *begin)) {
            return begin;
        }
        while (end - begin > 1) {
            RAIterator middle = begin + (end - begin) / 2;
            if (cmp(*middle, val)) {
                begin = middle;
            } else {
                end = middle;
            }
        }
        return end;
    }

    template <class RAIterator, class T = typename std::iterator_traits<RAIterator>::value_type, class Comparator>
    RAIterator adaptiveLowerBound(RAIterator begin, RAIterator end, const T &val, Comparator cmp) {
        if (begin == end || cmp(val, *begin)) {
            return begin;
        }
        std::size_t offset = 1, length = end - begin;
        while (offset < length && cmp(*(begin + offset), val)) {
            offset <<= 1;
        }
        return lowerBound(begin + offset / 2, begin + std::min(offset, length), val, cmp);
    }
}

#endif // TIMSORT_ADAPTIVE_LOWER_BOUND_H
