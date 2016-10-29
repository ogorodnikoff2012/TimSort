#ifndef __TIMSORT_ITERTOOLS_H__
#define __TIMSORT_ITERTOOLS_H__

#include <utility>
#include <iterator>
#include "utils.h"

namespace timsort {

    template <class RAIterator>
    void swapBlocks(RAIterator leftBlock, RAIterator rightBlock, std::size_t length) {
        using std::swap;
        while (length--) {
            swap(*leftBlock++, *rightBlock++);
        }
    }

    template <class BiDirIterator>
    void reverse(BiDirIterator begin, BiDirIterator end) {
        using std::swap;
        while ((begin != end) && (begin != --end)) {
            swap(*begin++, *end);
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
        IteratorReverser(const IteratorReverser<RAIterator> &it) : iter_(it.iter_) {}

        bool operator ==(const IteratorReverser<RAIterator> &other) {
            return iter_ == other.iter_;
        }
        bool operator !=(const IteratorReverser<RAIterator> &other) {
            return !operator ==(other);
        }

        reference operator *() const {
            return *(iter_ - 1);
        }
        pointer operator ->() const {
            return iter_ - 1;
        }

        IteratorReverser<RAIterator> &operator ++() {
            --iter_;
            return *this;
        }
        IteratorReverser<RAIterator> &operator --() {
            ++iter_;
            return *this;
        }
        const IteratorReverser<RAIterator> operator ++(int) {
            IteratorReverser<RAIterator> ans = *this;
            --iter_;
            return ans;
        }
        const IteratorReverser<RAIterator> operator --(int) {
            IteratorReverser<RAIterator> ans = *this;
            ++iter_;
            return ans;
        }
        
        const IteratorReverser<RAIterator> operator +(const difference_type diff) const { 
            return IteratorReverser<RAIterator>(iter_ - diff);
        }
        const IteratorReverser<RAIterator> operator -(const difference_type diff) const { 
            return IteratorReverser<RAIterator>(iter_ + diff);
        }
        difference_type operator -(const IteratorReverser<RAIterator> &other) const {
            return other.iter_ - iter_;
        }

        bool operator <(const IteratorReverser<RAIterator> &other) const {
            return other.iter_ < iter_;
        }
        bool operator >(const IteratorReverser<RAIterator> &other) const {
            return other.iter_ > iter_;
        }
        bool operator <=(const IteratorReverser<RAIterator> &other) const {
            return other.iter_ <= iter_;
        }
        bool operator >=(const IteratorReverser<RAIterator> &other) const {
            return other.iter_ >= iter_;
        }

        IteratorReverser<RAIterator> &operator +=(const difference_type diff) {
            iter_ -= diff;
            return *this;
        }
        IteratorReverser<RAIterator> &operator -=(const difference_type diff) {
            iter_ += diff;
            return *this;
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

}

#endif // __TIMSORT_ITERTOOLS_H__
