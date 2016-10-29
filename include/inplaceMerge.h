#ifndef __TIMSORT_INPLACE_MERGE_H__
#define __TIMSORT_INPLACE_MERGE_H__

#include "merge.h"
#include "params.h"
#include "slowsort.h"

namespace timsort {
    template <class RAIterator, class Comparator>
    void inplaceMerge(RAIterator begin, RAIterator border, RAIterator end,
            Comparator cmp, const ITimSortParams &params) {
        std::size_t length = std::distance(begin, end);

        if (length < 4) {
            selectionSort(begin, end, cmp);
            return;
        }

        std::size_t blockLength = sqrt(length);
        std::size_t blocksCount = length / blockLength;
        std::size_t secondRun = std::distance(border, begin);
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

            mergeWithBuffer(leftBlockBegin, rightBlockBegin, rightBlockBegin + blockLength,
                    lastBlockBegin, cmp, params);
        }

        std::size_t lastBigBlockLength = length - (blocksCount - 1) * blockLength;
        RAIterator doubleBigBlockBegin = begin + length - 2 * lastBigBlockLength;
        selectionSort(doubleBigBlockBegin, end, cmp);

        backwardMergeWithBuffer(begin, doubleBigBlockBegin, doubleBigBlockBegin + lastBigBlockLength,
                doubleBigBlockBegin + lastBigBlockLength, cmp, params);
        selectionSort(doubleBigBlockBegin + lastBigBlockLength, end, cmp);
    }
}

#endif // __TIMSORT_INPLACE_MERGE_H__
