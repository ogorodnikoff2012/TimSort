//
// Created by xenon on 10/30/16.
//

#include <tests.h>
#include <timsort.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>

/*
template <class Func>
double measureTime(Func f) {
    using namespace std::chrono;
    steady_clock::time_point t1 = steady_clock::now();
    f();
    steady_clock::time_point t2 = steady_clock::now();
    return duration_cast<milliseconds>(t2 - t1).count();
}
*/

bool testVector(int vectorSize) {
    using namespace std::chrono;
    std::vector<int> v(vectorSize);
    v[0] = testing::rand(true);
    for (int i = 1; i < vectorSize; ++i) {
        v[i] = rand();
    }
    std::vector<int> v2(v);

    steady_clock::time_point stdBegin = steady_clock::now();
    std::sort(v2.begin(), v2.end());
    steady_clock::time_point stdEnd = steady_clock::now();

    steady_clock::time_point TimBegin = steady_clock::now();
    TimSort(v.begin(), v.end());
    steady_clock::time_point TimEnd = steady_clock::now();

    double stdSortTimeMs = duration_cast<nanoseconds>(stdEnd - stdBegin).count() / 1e6;
    double TimSortTimeMs = duration_cast<nanoseconds>(TimEnd - TimBegin).count() / 1e6;

    bool ok = true;
    for (int i = 0; i < vectorSize; ++i) {
        ok &= v[i] == v2[i];
    }
    if (ok) {
        std::cout << "std::sort time is " << stdSortTimeMs << " ms." << std::endl;
        std::cout << "TimSort time is " << TimSortTimeMs << " ms." << std::endl;
        std::cout << "std::sort is " << TimSortTimeMs / stdSortTimeMs << " times faster than TimSort" << std::endl;
    }
    return ok;
}

bool testArray(int arraySize) {
    using namespace std::chrono;
    int *arr = new int[arraySize];
    arr[0] = testing::rand(true);
    for (int i = 1; i < arraySize; ++i) {
        arr[i] = rand();
    }
    int *arr2 = new int[arraySize];
    for (int i = 0; i < arraySize; ++i) {
        arr2[i] = arr[i];
    }

    steady_clock::time_point stdBegin = steady_clock::now();
    std::sort(arr, arr + arraySize);
    steady_clock::time_point stdEnd = steady_clock::now();

    steady_clock::time_point TimBegin = steady_clock::now();
    TimSort(arr2, arr2 + arraySize);
    steady_clock::time_point TimEnd = steady_clock::now();

    double stdSortTimeMs = duration_cast<nanoseconds>(stdEnd - stdBegin).count() / 1e6;
    double TimSortTimeMs = duration_cast<nanoseconds>(TimEnd - TimBegin).count() / 1e6;

    bool ok = true;
    for (int i = 0; i < arraySize; ++i) {
        ok &= arr[i] == arr2[i];
    }
    if (ok) {
        std::cout << "std::sort time is " << stdSortTimeMs << " ms." << std::endl;
        std::cout << "TimSort time is " << TimSortTimeMs << " ms." << std::endl;
        std::cout << "std::sort is " << TimSortTimeMs / stdSortTimeMs << " times faster than TimSort" << std::endl;
    }
    delete[] arr;
    delete[] arr2;
    return ok;
}



namespace testing {
    Test tests[] = {
            Test{"Vector of random values (50)", std::bind(testVector, 50)},
            Test{"Array of random values (50)", std::bind(testArray, 50)},
            Test{"Vector of random values (100)", std::bind(testVector, 100)},
            Test{"Array of random values (100)", std::bind(testArray, 100)},
            Test{"Vector of random values (1000)", std::bind(testVector, 1000)},
            Test{"Array of random values (1000)", std::bind(testArray, 1000)},
            Test{"Vector of random values (10000)", std::bind(testVector, 10000)},
            Test{"Array of random values (10000)", std::bind(testArray, 10000)},
            Test{"Vector of random values (100000)", std::bind(testVector, 100000)},
            Test{"Array of random values (100000)", std::bind(testArray, 100000)},
            Test{"Vector of random values (1000000)", std::bind(testVector, 1000000)},
            Test{"Array of random values (1000000)", std::bind(testArray, 1000000)},
            Test{"Vector of random values (1000000)", std::bind(testVector, 1000000)},
            Test{"Array of random values (1000000)", std::bind(testArray, 1000000)},
    };
    std::size_t testsCount = sizeof(tests) / sizeof(Test);

    bool doTest(const Test &test) {
        std::cout << test.description << ": " << std::endl;
        bool ok = test.testFunc();
        std::cout << (ok ? "OK" : "Failed") << std::endl;
        return ok;
    }

    int rand(bool reset) {
        const int A = 84589, C = 45989, M = 217728; // From wikipedia
        static long long int x = 0;
        if (reset) {
            x = 0;
        }
        x = (A * x + C) % M;
        return static_cast<int>(x);
    }

}