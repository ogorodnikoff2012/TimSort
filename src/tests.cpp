//
// Created by xenon on 10/30/16.
//

#include <tests.h>
#include <timsort.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>

template <class Func>
double measureTime(Func f) {
    using namespace std::chrono;
    steady_clock::time_point t1 = steady_clock::now();
    f();
    steady_clock::time_point t2 = steady_clock::now();
    return duration_cast<milliseconds>(t2 - t1).count();
}

bool testVector() {
    using namespace std::chrono;
    const int VECTOR_SIZE = 100;
    std::vector<int> v(VECTOR_SIZE);
    const int A = 17, B = 19, C = 1009; // needed for pseudo-random numbers generating
    v[0] = 17;
    for (int i = 1; i < VECTOR_SIZE; ++i) {
        v[i] = (A * v[i - 1] + B) % C;
    }
    std::vector<int> v2(v);

    steady_clock::time_point stdBegin = steady_clock::now();
    std::sort(v2.begin(), v2.end());
    steady_clock::time_point stdEnd = steady_clock::now();

    steady_clock::time_point TimBegin = steady_clock::now();
    TimSort(v.begin(), v.end());
    steady_clock::time_point TimEnd = steady_clock::now();

    double stdSortTime = duration_cast<microseconds>(stdEnd - stdBegin).count();
    double TimSortTime = duration_cast<microseconds>(TimEnd - TimBegin).count();

    bool ok = true;
    for (int i = 0; i < VECTOR_SIZE; ++i) {
        ok &= v[i] == v2[i];
    }
    if (ok) {
        std::cout << "std::sort time is " << stdSortTime << " us." << std::endl;
        std::cout << "TimSort time is " << TimSortTime << " us." << std::endl;
    }
    return ok;
}

namespace testing {
    Test tests[] = {Test{"Vector of random values", testVector}};
    std::size_t testsCount = sizeof(tests) / sizeof(Test);

    bool doTest(const Test &test) {
        std::cout << test.description << ": " << std::endl;
        bool ok = test.testFunc();
        std::cout << (ok ? "OK" : "Failed") << std::endl;
        return ok;
    }
}