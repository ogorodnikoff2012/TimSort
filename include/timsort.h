#ifndef TIMSORT_TIMSORT_H
#define TIMSORT_TIMSORT_H

#include "list.h"
#include "utils.h"
#include "itertools.h"
#include "merge.h"
#include "inplaceMerge.h"
#include "slowsort.h"
#include "params.h"
#include "debug.h"

#include <iterator>

namespace timsort {
    template <class RAIterator>
    struct TRun {
        RAIterator begin, end;
        std::size_t dummyLength;
        TRun(RAIterator begin, RAIterator end, std::size_t dummyLength = 0) : begin(begin), 
                end(end), dummyLength(dummyLength) {  }
        TRun(const TRun<RAIterator> &run) : begin(run.begin), end(run.end), dummyLength(run.dummyLength) {}
        std::size_t length() const {
            return dummyLength ? dummyLength : std::distance(begin, end);
        }
        bool dummy() const {
            return dummyLength; // != 0
        }
    };

    template <class RAIterator>
    std::ostream &operator <<(std::ostream &o, const TRun<RAIterator> &run) {
        o << "Length: " << run.length() << ' ';
        if (run.dummy()) {
            o << "<dummy>";
        //} else {
        //    __ostr_iter<RAIterator>(o, run.begin) << run.end;
        }
        return o;
    }

    template <class RAIterator, class Comparator>
    void appendRun(List<TRun<RAIterator>> &runList, 
            typename List<TRun<RAIterator>>::iterator newRun, Comparator cmp,
            const ITimSortParams &params) {
#ifdef DEBUG
        IT_PRINT(Runs, runList.begin(), ++newRun);
        --newRun;
#endif
        auto itZ = newRun, itY = newRun, itX = newRun;
        std::advance(itY, -1);
        std::advance(itX, -2);

        EWhatMerge whatMerge;

        if (itY->dummy()) {
            whatMerge = WM_NoMerge;
        } else if (itX->dummy()) {
            if (params.needMerge(itY->length(), itZ->length())) {
                whatMerge = WM_MergeYZ;
            } else {
                whatMerge = WM_NoMerge;
            }
        } else {
            whatMerge = params.whatMerge(itX->length(), itY->length(), itZ->length());
        }

        switch (whatMerge) {
            case WM_NoMerge:
                break;
            case WM_MergeYZ:
                inplaceMerge(itY->begin, itY->end, itZ->end, cmp, params);
                itZ->begin = itY->begin;
                runList.erase(itY);
                appendRun(runList, itZ, cmp, params);
                break;
            case WM_MergeXY:
                inplaceMerge(itX->begin, itX->end, itY->end, cmp, params);
                itX->end = itY->end;
                runList.erase(itY);
                appendRun(runList, itX, cmp, params);
                appendRun(runList, itZ, cmp, params);
                break;
        }
    }
};

// template arguments are copied from http://stackoverflow.com/questions/2447458/
template <class RAIterator, class Comparator>
void TimSort(RAIterator begin, RAIterator end, Comparator cmp, const timsort::ITimSortParams &params) {
    using timsort::TRun;
    using timsort::List;
    using timsort::insertionSort;

    typedef TRun<RAIterator> Run;

    // step 0: get minimum run length
    std::size_t length = std::distance(begin, end);
    std::size_t minrun = params.minRun(length);
    
    // step 1: split array into runs
    List<Run> runs;

    // Add dummy runs
    runs.pushBack(Run(begin, end, 2 * length + 2));
    runs.pushBack(Run(begin, end, length + 1));
    
    RAIterator runBegin = begin;
    while (runBegin != end) {
        RAIterator runEnd = runBegin + 1;
        std::size_t runLength = 1;
        if (runEnd == end) {
            runs.pushBack(Run(runBegin, runEnd));
            runBegin = end;
            break;
        }

        ++runEnd;
        ++runLength;
        bool isIncreasing = !cmp(*(runBegin + 1), *runBegin); // !(B < A) == A <= B, 
                                                              // non-strict increasing
        bool currentRunSorted = true;
        while (runEnd != end && runLength < minrun) {
            ++runEnd;
            ++runLength;
            if (currentRunSorted && !cmp(*(runEnd - 1), *(runEnd - 2)) != isIncreasing) {
                currentRunSorted = false;
            }
        }

        if (currentRunSorted) {
            while (runEnd != end && !cmp(*runEnd, *(runEnd - 1)) == isIncreasing) {
                ++runEnd;
                ++runLength;
            }
            if (!isIncreasing) {
                timsort::reverse(runBegin, runEnd);
            }
        } else {
            insertionSort(runBegin, runEnd, cmp);
        }
        runs.pushBack(Run(runBegin, runEnd));
        runBegin = runEnd;
    }
    
    // step 2: merging
    auto runIterator = runs.begin();
    std::advance(runIterator, 2);
    while (runIterator != runs.end()) {
        appendRun(runs, runIterator, cmp, params);
        ++runIterator;
    }

    while (runs.size() > 3) {
        Run rightRun = runs.back();
        runs.popBack();
        Run &leftRun = runs.back();
        inplaceMerge(leftRun.begin, leftRun.end, rightRun.end, cmp, params);
        leftRun.end = rightRun.end;
    }
}

template <class RAIterator>
void TimSort(RAIterator begin, RAIterator end, const timsort::ITimSortParams &params) {
    TimSort(begin, end, std::less<typename std::iterator_traits<RAIterator>::value_type>(), params);
}

template <class RAIterator, class Comparator, class = 
        typename std::enable_if<!std::is_base_of<timsort::ITimSortParams, Comparator>::value>::type>
void TimSort(RAIterator begin, RAIterator end, Comparator cmp) {
    TimSort(begin, end, cmp, timsort::StdTimSortParams());
}

template <class RAIterator>
void TimSort(RAIterator begin, RAIterator end) {
    TimSort(begin, end, std::less<typename std::iterator_traits<RAIterator>::value_type>(), 
            timsort::StdTimSortParams());
}

#endif // TIMSORT_TIMSORT_H
