#ifndef TIMSORT_LIST_H
#define TIMSORT_LIST_H

// begin(), end(), push_back(), pop_back(), back(), erase()

#include <iterator>

namespace timsort {
    template <class T>
    class List {
    private:
        struct ListNode {
            T *data;
            ListNode *left, *right;
            ListNode() : data(nullptr), left(nullptr), right(nullptr) {}
            ~ListNode() {
                if (data != nullptr) {
                    delete data;
                }
            }
        };

        static void connect(ListNode *left, ListNode *right) {
            left->right = right;
            right->left = left;
        }

        static void insertBefore(ListNode *node, ListNode *what) {
            connect(node->left, what);
            connect(what, node);
        }

        static void eraseBefore(ListNode *node) {
            ListNode *toBeErased = node->left;
            connect(toBeErased->left, node);
            delete toBeErased;
        }

        ListNode *pre_begin_, *end_;
        std::size_t size_;
    public:
        typedef T value_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef value_type *pointer;
        typedef const value_type *const_pointer;

        List() : pre_begin_(new ListNode), end_(new ListNode), size_(0) {
            pre_begin_->right = end_;
            end_->left = pre_begin_;
        } 
        class iterator {
        private:
            ListNode *cur_;
            iterator(ListNode *cur) : cur_(cur) {}
        public:
            typedef std::ptrdiff_t difference_type;
            typedef T value_type;
            typedef T *pointer;
            typedef T &reference;
            typedef std::bidirectional_iterator_tag iterator_category;
            bool operator ==(const iterator &other) const {
                return cur_ == other.cur_;
            }
            bool operator !=(const iterator &other) const {
                return !operator ==(other);
            }
            pointer operator ->() const {
                return cur_->data;
            }
            reference operator *() const {
                return *cur_->data;
            }
            iterator &operator ++() {
                cur_ = cur_->right;
                return *this;
            }
            const iterator operator ++(int) {
                iterator ans = *this;
                ++(*this);
                return ans;
            }
            iterator &operator --() {
                cur_ = cur_->left;
                return *this;
            }
            const iterator operator --(int) {
                iterator ans = *this;
                --(*this);
                return ans;
            }
            friend class List<T>;
        };
        iterator begin() const {
            return iterator(pre_begin_->right);
        }
        iterator end() const {
            return iterator(end_);
        }
        void pushBack(const T &x) {
            ListNode *node = new ListNode;
            node->data = new T(x);
            insertBefore(end_, node);
            ++size_;
        }
        T &back() const {
            return *(end_->left->data);
        }
        void popBack() {
            eraseBefore(end_);
            --size_;
        }
        std::size_t size() const {
            return size_;
        }
        void erase(iterator iter) {
            ListNode *node = iter.cur_;
            connect(node->left, node->right);
            delete node;
            --size_;
        }
    };
}

#endif // TIMSORT_LIST_H
