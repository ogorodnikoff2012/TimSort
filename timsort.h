#include <list>
#include <iterator>
#include <algorithm>
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

    std::size_t getMinrun(std::size_t length) {
        std::size_t r = 0;
        while (length >= 64) {
            r |= length & 1;
            length >>= 1;
        }
        return length + r;
    }

    double abs(double x) {
        return x < 0 ? -x : x;
    }

    unsigned long long sqrt(unsigned long long n) {
        double a = 1e6; // Just a random initial value
        while (abs(a * a - n) > 1) {
            a = (a + n / a) / 2;
        }
        unsigned long long ans = a + 3;
        while (ans * ans > n) {
            --ans;
        }
        return ans;
    }

    template <class RAIterator>
    void swapBlocks(RAIterator leftBlock, RAIterator rightBlock, std::size_t length) {
        while (length--) {
            std::swap(*leftBlock++, *rightBlock++);
        }
    }

    template <class RAIterator, class Comparator>
    struct Block {
        RAIterator begin;
        std::size_t length;
        Comparator cmp;
        Block(RAIterator begin, std::size_t length, Comparator cmp) : begin(begin), length(length),
                cmp(cmp) {}
        void swap(const Block<RAIterator, Comparator> &other) const {
            swapBlocks(begin, other.begin, length);
        }
        bool operator <(const Block<RAIterator, Comparator> &other) const {
            bool c1 = cmp(*begin, *other.begin);
            if (!c1) {
                return !cmp(*other.begin, *begin) && cmp(*(begin + length - 1), 
                        *(other.begin + other.length - 1));
            }
            return c1;
        }

    };

    template <class RAIterator, class Comparator>
    void swap(Block<RAIterator, Comparator> &b1, Block<RAIterator, Comparator> &b2) {
        b1.swap(b2);
    }

    template <class RAIterator, class Comparator>
    class BlockIterator {
    private:
        Block<RAIterator, Comparator> curBlock;
    public:
        typedef typename std::iterator_traits<RAIterator>::value_type value_type;
        typedef typename std::iterator_traits<RAIterator>::reference reference;
        typedef typename std::iterator_traits<RAIterator>::pointer pointer;
        typedef typename std::iterator_traits<RAIterator>::difference_type difference_type;
        typedef std::random_access_iterator_tag iterator_category;
        
        BlockIterator(const Block<RAIterator, Comparator> block) : curBlock(block) {}
        Block<RAIterator, Comparator> &operator *() {
            return curBlock;
        }
        bool operator !=(const BlockIterator<RAIterator, Comparator> &iter) const {
            return curBlock.begin != iter.curBlock.begin;
        }
        BlockIterator<RAIterator, Comparator> &operator ++() {
            curBlock.begin += curBlock.length;
            return *this;
        }
        BlockIterator<RAIterator, Comparator> operator ++(int) {
            auto copy = *this;
            ++(*this);
            return copy;
        }
        BlockIterator<RAIterator, Comparator> operator +=(std::size_t n) {
            curBlock.begin += n * curBlock.length;
            return *this;
        }
        const BlockIterator<RAIterator, Comparator> operator +(std::size_t n) {
            BlockIterator<RAIterator, Comparator> ans = *this;
            ans += n;
            return ans;
        }
    };
    
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

    template <class RAIterator>
    class IteratorReverser {
    private:
        RAIterator iter_;
    public:
        typedef typename std::iterator_traits<RAIterator>::value_type value_type;
        typedef typename std::iterator_traits<RAIterator>::reference reference;
        typedef typename std::iterator_traits<RAIterator>::pointer pointer;
        typedef typename std::iterator_traits<RAIterator>::difference_type difference_type;
        typedef std::random_access_iterator_tag iterator_category;

        IteratorReverser(const RAIterator &it) : iter_(it) {}
        value_type &operator *() const {
            return *(iter_ - 1);
        }
        const IteratorReverser<RAIterator> operator ++(int) {
            IteratorReverser<RAIterator> ans = *this;
            --iter_;
            return ans;
        }
        const IteratorReverser<RAIterator> operator +(const difference_type diff) const { 
            return IteratorReverser<RAIterator>(iter_ - diff);
        }
        bool operator ==(const IteratorReverser<RAIterator> &other) {
            return iter_ == other.iter_;
        }
        bool operator !=(const IteratorReverser<RAIterator> &other) {
            return !operator ==(other);
        }
        difference_type operator -(const IteratorReverser<RAIterator> &other) const {
            return other.iter_ - iter_;
        }
    };

    template <class RAIterator>
    const IteratorReverser<RAIterator> reverseIter(const RAIterator &iter) {
        return IteratorReverser<RAIterator>(iter);
    }

    template <class Comparator, class T>
    class InvertedComparator {
    private:
        Comparator cmp_;
    public:
        InvertedComparator(const Comparator &cmp) : cmp_(cmp) {}
        bool operator() (const T &a, const T &b) {
            return cmp_(b, a);
        }
    };

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
    void appendRun(std::list<TRun<RAIterator>> &runList, 
            typename std::list<TRun<RAIterator>>::iterator newRun, Comparator cmp) {
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
    std::list<Run> runs;

    // Add dummy runs
    runs.push_back(Run(begin, end, 2 * length + 2));
    runs.push_back(Run(begin, end, length + 1));
    
    RAIterator runBegin = begin;
    while (runBegin != end) {
        RAIterator runEnd = runBegin + 1;
        std::size_t runLength = 1;
        if (runEnd == end) {
            runs.push_back(Run(runBegin, runEnd));
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
                std::reverse(runBegin, runEnd);
            }
        } else {
            insertionSort(runBegin, runEnd, cmp);
        }
        runs.push_back(Run(runBegin, runEnd));
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
        runs.pop_back();
        Run &leftRun = runs.back();
        inplaceMerge(leftRun, rightRun, cmp);
        leftRun.end = rightRun.end;
    }
}
