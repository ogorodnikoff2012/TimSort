#ifndef __TIMSORT_TIMSORT_H__
#define __TIMSORT_TIMSORT_H__

#include "list.h"
#include <iterator>
#include "utils.h"
#include "itertools.h"
#include "merge.h"
#include "slowsort.h"
#include <stdexcept>

namespace timsort {
    template <class RAIterator>
    struct TRun {
        RAIterator begin, end;
        std::size_t dummyLength;
        TRun(RAIterator begin, RAIterator end, std::size_t dummyLength = 0) : begin(begin), 
                end(end), dummyLength(dummyLength) {  }
        std::size_t length() const {
            return dummyLength ? dummyLength : std::distance(begin, end);
        }
        bool dummy() const {
            return dummyLength; // != 0
        }
    };

    template <class RAIterator, class Comparator>
    void inplaceMerge(const TRun<RAIterator> &runLeft, const TRun<RAIterator> &runRight,
            Comparator cmp) {
        if (runLeft.dummy() || runRight.dummy() || runLeft.end != runRight.begin) {
            throw std::runtime_error("Incorrect inplace merge");
        }
        RAIterator begin = runLeft.begin, end = runRight.end;
        std::size_t length = std::distance(begin, end);

        if (length < 4) {
            selectionSort(begin, end, cmp);
            return;
        }

        std::size_t blockLength = sqrt(length);
        std::size_t blocksCount = length / blockLength;
        std::size_t secondRun = runLeft.length();
        std::size_t blockWithFirstRunEnd = (secondRun - 1) / blockLength;
        RAIterator lastBlockBegin = begin + (blocksCount - 1) * blockLength;
        
        swapBlocks(begin + blockWithFirstRunEnd * blockLength, lastBlockBegin, blockLength);

        selectionSort(BlockIterator<RAIterator, Comparator>(
                    Block<RAIterator, Comparator>(begin, blockLength, cmp)),
                BlockIterator<RAIterator, Comparator>(
                    Block<RAIterator, Comparator>(lastBlockBegin, blockLength, cmp)), 
                std::less<Block<RAIterator, Comparator>>());

        for (std::size_t i = 0; i < blocksCount - 2; ++i) {
            // Merge i & i+1 blocks using last block as buffer
            RAIterator leftBlockBegin = begin + i * blockLength;
            RAIterator rightBlockBegin = leftBlockBegin + blockLength;

            mergeWithBuffer(leftBlockBegin, leftBlockBegin + blockLength,
                    rightBlockBegin, rightBlockBegin + blockLength, lastBlockBegin, cmp);
        }

        std::size_t lastBigBlockLength = length - (blocksCount - 1) * blockLength;
        RAIterator doubleBigBlockBegin = begin + length - 2 * lastBigBlockLength;
        selectionSort(doubleBigBlockBegin, end, cmp);

        backwardMergeWithBuffer(begin, doubleBigBlockBegin,
                doubleBigBlockBegin, doubleBigBlockBegin + lastBigBlockLength,
                doubleBigBlockBegin + lastBigBlockLength, cmp);
        selectionSort(doubleBigBlockBegin + lastBigBlockLength, end, cmp);
    }

    template <class RAIterator, class Comparator>
    void appendRun(List<TRun<RAIterator>> &runList, 
            typename List<TRun<RAIterator>>::iterator newRun, Comparator cmp) {
        auto itZ = newRun, itY = newRun, itX = newRun;
        std::advance(itY, -1);
        std::advance(itX, -2);
        if (itZ->length() >= itY->length()) {
            inplaceMerge(*itY, *itZ, cmp);
            itZ->begin = itY->begin;
            runList.erase(itY);
            appendRun(runList, itZ, cmp);
        } else if (itZ->length() + itY->length() > itX->length()) {
            inplaceMerge(*itX, *itY, cmp);
            itX->end = itY->end;
            runList.erase(itY);
            appendRun(runList, itX, cmp);
            appendRun(runList, itZ, cmp);
        }
    }
};

// template arguments are copied from http://stackoverflow.com/questions/2447458/
template <class RAIterator, class Comparator = std::less<
    typename std::iterator_traits<RAIterator>::value_type> >
void TimSort(RAIterator begin, RAIterator end, Comparator cmp = Comparator()) {
    using namespace timsort;
    typedef TRun<RAIterator> Run;

    // step 0: get minimum run length
    std::size_t length = std::distance(begin, end);
    std::size_t minrun = getMinrun(length);
    
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
            if (currentRunSorted && cmp(*(runEnd - 2), *(runEnd - 1)) != isIncreasing) {
                currentRunSorted = false;
            }
        }

        if (currentRunSorted) {
            while (runEnd != end && cmp(*(runEnd - 1), *runEnd) == isIncreasing) {
                ++runEnd;
                ++runLength;
            }
            if (!isIncreasing) {
                reverse(runBegin, runEnd);
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
        appendRun(runs, runIterator, cmp);
        ++runIterator;
    }

    while (runs.size() > 3) {
        Run rightRun = runs.back();
        runs.popBack();
        Run &leftRun = runs.back();
        inplaceMerge(leftRun, rightRun, cmp);
        leftRun.end = rightRun.end;
    }
}

#endif // __TIMSORT_TIMSORT_H__
