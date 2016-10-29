#ifdef _DEBUG_STREAM
#undef _DEBUG_STREAM
#endif

#ifdef _DEBUG_STDERR
#define _DEBUG_STREAM std::cerr
#else
#define _DEBUG_STREAM std::cout
#endif // _DEBUG_STDOUT

#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef DEBUG
#include <iostream>

#define PRINT(x) _DEBUG_STREAM << #x << ": " << (x) << std::endl;
#define IT_PRINT(name, begin, end) _DEBUG_STREAM << #name << ": " << (begin) << (end) << std::endl;
#define WRITE(x) _DEBUG_STREAM << x << std::endl;
#define DELIM WRITE("=========================================");

#else

#define PRINT(x)
#define IT_PRINT(name, begin, end)
#define WRITE(x)
#define DELIM
#endif // DEBUG

#define PRINTABLE(T) friend std::ostream &operator <<(std::ostream &o, const T &obj);

#define PRINTABLE_BEGIN(T) std::ostream &operator <<(std::ostream &o, const T &obj) \
    {\
        bool first_element = true;\
        o << "{";

#define FIELD(F) if (!first_element) \
        {\
            o << ", ";\
        }\
        first_element = false;\
        o << #F << ": " << obj. F;

#define IT_FIELD(FNAME, BEGIN, END) if (!first_element) \
        {\
            o << ", ";\
        }\
        first_element = false;\
        o << #FNAME << ": " << obj. BEGIN << obj. END;

#define PRINTABLE_END return o << "}";\
    }

#include <utility>
#include <iterator>

template<class A, class B>
std::ostream &operator <<(std::ostream &o, const std::pair<A, B> &p)
{
    return o << '(' << p.first << ", " << p.second << ')';
}

template <class T>
struct __ostr_iter 
{
    std::ostream &out;
    T it;

    __ostr_iter(std::ostream &_out, T _it) : 
        out(_out), 
        it(_it) 
    {}
};

template <class T>
struct __printable
{
    T &container;
    __printable(T &container) :
        container(container)
    {}
};

template <class T>
__printable<T> _pr(T &container)
{
    return __printable<T>(container);
}

template <class T>
std::ostream &operator <<(std::ostream &out, const __printable<T> p)
{
    return out << p.container.begin() << p.container.end();
}

template <
            class T, 
            class = typename std::iterator_traits<T>::iterator_category, 
            class = typename std::enable_if<!std::is_pointer<T>::value && !std::is_array<T>::value>::type
         >
struct is_iterator {
    const static bool value = true;
};

template <class T>
struct is_iterator<T, void, void> {
    const static bool value = false;
};

template <class T>
struct is_c_str
    : std::integral_constant<
        bool, 
        std::is_same<char const *, typename std::decay<T>::type>::value ||
        std::is_same<char *, typename std::decay<T>::type>::value
>{};

template <typename T, typename E>
struct has_c_str_method
{
    template <typename U, E (U::*)() const> struct SFINAE {};
    template <typename U> static char Test(SFINAE<U, &U::c_str>*);
    template <typename U> static int Test(...);
    static const bool has = sizeof(Test<T>(0)) == sizeof(char);
};

template <typename T>
struct has_the_container_c_str_method :
    has_c_str_method<T, typename T::const_pointer>
{};

template <class T, class = typename std::enable_if<is_iterator<T>::value>::type>
__ostr_iter<T> operator <<(std::ostream &out, T it) 
{
    return __ostr_iter<T>(out, it);
}


template <class T, class = decltype(std::declval<T>().begin()), class = decltype(std::declval<T>().end()),
        class = typename std::enable_if<!has_the_container_c_str_method<T>::has>::type>
std::ostream &operator <<(std::ostream &out, const T &collection)
{
    return out << collection.begin() << collection.end();
}

template <class T>
class ArrayIterator {
public:
	typedef std::size_t difference_type;
	typedef T value_type;
	typedef T *pointer;
	typedef T &reference;
	typedef std::random_access_iterator_tag iterator_category;
	ArrayIterator(pointer ptr) : ptr(ptr) {}
	reference operator *() const {
		return *ptr;
	}
	const ArrayIterator<T> operator ++(int) {
		ArrayIterator<T> it = *this;
		++ptr;
		return it;
	}
	bool operator ==(const ArrayIterator<T> &it) const {
		return ptr == it.ptr;
	}
	bool operator !=(const ArrayIterator<T> &it) const {
		return !operator ==(it);
	}
private:
	pointer ptr;
};

template <class T>
class Array {
public:
	typedef T value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type &reference;
	typedef const value_type &const_reference;
	typedef value_type *pointer;
	typedef const value_type *const_pointer;
	typedef ArrayIterator<T> iterator;
	Array(pointer ptr, difference_type length) : ptr(ptr), length(length) {}
	iterator begin() const {
		return ArrayIterator<T>(ptr);
	}
	iterator end() const {
		return ArrayIterator<T>(ptr + length);
	}
private:
	pointer ptr;
	difference_type length;
};

template<class T>
std::ostream &operator <<(__ostr_iter<T> oi, T it)
{
    oi.out << "[";
    if (oi.it == it) 
    {
        return oi.out << "]";
    }
    
    oi.out << *oi.it++;
    while (oi.it != it) 
    {
        oi.out << ", " << *oi.it++;
    }
    return oi.out << "]";
}

#endif // __DEBUG_H__
#include <ostream>
#include <iterator>

