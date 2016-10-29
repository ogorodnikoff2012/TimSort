#include "timsort.h"
#include <algorithm>
#include <vector>
#include <iostream>

using std::vector;
using std::ostream;
using std::cin;
using std::cout;
using std::endl;

template <class T>
ostream &operator <<(ostream &o, const vector<T> &v) {
    o << '[';
    if (!v.empty()) {
        o << v[0];
        for (auto it = v.begin() + 1; it != v.end(); ++it) {
            o << ", " << *it;
        }
    }
    return o << ']';
}

template <class T>
const T input() {
    T x;
    cin >> x;
    return x;
}

int main() {
    int a = 3, b = 17, c = 179;
    int size = input<int>();
    vector<int> v(size);
    v[0] = ((input<int>() % c)) + c % c;
    for (int i = 1; i < size; ++i) {
        v[i] = (a * v[i - 1] + b) % c;
    }
    vector<int> v2(v);
#ifndef NO_OUTPUT
    cout << v << endl;
#endif
    TimSort(v.begin(), v.end());
    std::sort(v2.begin(), v2.end());
#ifndef NO_OUTPUT
    cout << v << endl;
    cout << v2 << endl;
#endif
    return 0;
}
