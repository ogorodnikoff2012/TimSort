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

template <class RAIter1, class RAIter2, class Comparator>
bool compareSorts(RAIter1 beginTim, RAIter1 endTim, RAIter2 beginStd, RAIter2 endStd, Comparator cmp, const timsort::ITimSortParams &params) {
    using namespace std::chrono;
    steady_clock::time_point stdBegin = steady_clock::now();
    std::sort(beginStd, endStd, cmp);
    steady_clock::time_point stdEnd = steady_clock::now();

    steady_clock::time_point TimBegin = steady_clock::now();
    TimSort(beginTim, endTim, cmp, params);
    steady_clock::time_point TimEnd = steady_clock::now();

    double stdSortTimeMs = duration_cast<nanoseconds>(stdEnd - stdBegin).count() / 1e6;
    double TimSortTimeMs = duration_cast<nanoseconds>(TimEnd - TimBegin).count() / 1e6;

    bool ok = true;
    for (RAIter1 it1 = beginTim + 1; it1 != endTim; ++it1) {
        ok &= !cmp(*it1, *(it1 - 1));
    }

    if (ok) {
        std::cout << "std::sort time is " << stdSortTimeMs << " ms." << std::endl;
        std::cout << "TimSort time is " << TimSortTimeMs << " ms." << std::endl;
        std::cout << "std::sort is " << TimSortTimeMs / stdSortTimeMs << " times faster than TimSort" << std::endl;
    }
    return ok;

}

bool testVectorOfRandomInts(int vectorSize) {
    using namespace std::chrono;
    std::vector<int> v(vectorSize);
    v[0] = testing::rand(true);
    for (int i = 1; i < vectorSize; ++i) {
        v[i] = rand();
    }
    std::vector<int> v2(v);
    return compareSorts(v.begin(), v.end(), v2.begin(), v2.end(), std::less<int>(), timsort::StdTimSortParams());
}

bool testArrayOfRandomInts(int arraySize) {
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

    bool ok = compareSorts(arr, arr + arraySize, arr2, arr2 + arraySize, std::less<int>(), timsort::StdTimSortParams());
    delete[] arr;
    delete[] arr2;
    return ok;
}

bool testVariousVectorsOfRandomInts() {
    const int LENGTHS[] = {50, 100, 1000, 10000, 100000, 1000000};
    const int TESTS_COUNT = sizeof(LENGTHS) / sizeof(LENGTHS[0]);
    for (int i = 0; i < TESTS_COUNT; ++i) {
        std::cout << testing::SUBTEST_HEADER << (i + 1) << ". Length = " << LENGTHS[i] << ":" << std::endl;
        bool ok = testVectorOfRandomInts(LENGTHS[i]);
        std::cout << (ok ? testing::VERDICT_OK : testing::VERDICT_FAILED) << std::endl;
        if (!ok) {
            return false;
        }
    }
    return true;
}

bool testVariousArraysOfRandomInts() {
    const int LENGTHS[] = {50, 100, 1000, 10000, 100000, 1000000};
    const int TESTS_COUNT = sizeof(LENGTHS) / sizeof(LENGTHS[0]);
    for (int i = 0; i < TESTS_COUNT; ++i) {
        std::cout << testing::SUBTEST_HEADER << (i + 1) << ". Length = " << LENGTHS[i] << ":" << std::endl;
        bool ok = testArrayOfRandomInts(LENGTHS[i]);
        std::cout << (ok ? testing::VERDICT_OK : testing::VERDICT_FAILED) << std::endl;
        if (!ok) {
            return false;
        }
    }
    return true;
}

bool testPartiallySortedVector(int length) {
    std::vector<int> v(length);
    const int SWAPS = 1000;
    for (int i = 0; i < length; ++i) {
        v[i] = i;
    }
    for (int swaps = 0; swaps < SWAPS; ++swaps) {
        std::swap(v[testing::rand(swaps == 0) % length], v[testing::rand() % length]);
    }
    std::vector<int> v2(v);
    return compareSorts(v.begin(), v.end(), v2.begin(), v2.end(), std::less<int>(), timsort::StdTimSortParams());
}

bool testPartiallySortedVectors() {
    const int LENGTHS[] = {50, 100, 1000, 10000, 100000, 1000000};
    const int TESTS_COUNT = sizeof(LENGTHS) / sizeof(LENGTHS[0]);
    for (int i = 0; i < TESTS_COUNT; ++i) {
        std::cout << testing::SUBTEST_HEADER << (i + 1) << ". Length = " << LENGTHS[i] << ":" << std::endl;
        bool ok = testPartiallySortedVector(LENGTHS[i]);
        std::cout << (ok ? testing::VERDICT_OK : testing::VERDICT_FAILED) << std::endl;
        if (!ok) {
            return false;
        }
    }
    return true;
}

bool testConcatenatedSortedVectors(int partLength, int partsCount) {
    int length = partLength * partsCount;
    std::vector<int> v(length);
    v[0] = testing::rand(true);
    for (int i = 1; i < length; ++i) {
        v[i] = testing::rand();
    }
    for (int i = 0; i < partsCount; ++i) {
        std::sort(v.begin() + i * partLength, v.begin() + (i + 1) * partLength);
    }
    std::vector<int> v2(v);
    return compareSorts(v.begin(), v.end(), v2.begin(), v2.end(), std::less<int>(), timsort::StdTimSortParams());
}

bool testVariousConcatenatedSortedVectors() {
    const int PART_LENGTHS[] = {20, 40, 80, 128, 1024};
    const int PART_LENGTHS_LENGTH = sizeof(PART_LENGTHS) / sizeof(int);
    const int PART_COUNTS[]  = {2, 4, 10, 100, 1000, 10000};
    const int PART_COUNTS_LENGTH = sizeof(PART_COUNTS) / sizeof(int);
    int tests = 0;
    for (int i = 0; i < PART_LENGTHS_LENGTH; ++i) {
        for (int j = 0; j < PART_COUNTS_LENGTH; ++j) {
            int partLength = PART_LENGTHS[i];
            int partsCount = PART_COUNTS[j];
            int totalLength = partLength * partsCount;
            if (totalLength > 1e7) {
                continue;
            }
            std::cout << testing::SUBTEST_HEADER << (++tests) << ". Length = " << partLength * partsCount <<
                      ", parts count = " << partsCount << ":" << std::endl;
            bool ok = testConcatenatedSortedVectors(partLength, partsCount);
            std::cout << (ok ? testing::VERDICT_OK : testing::VERDICT_FAILED) << std::endl;
            if (!ok) {
                return false;
            }
        }
    }
    return true;
}

struct Point3D {
    typedef long long int ValueType;
    ValueType x, y, z;
    explicit Point3D(ValueType x = ValueType(), ValueType y = ValueType(), ValueType z = ValueType()) : x(x), y(y), z(z) {}
    Point3D(const Point3D &p) : x(p.x), y(p.y), z(p.z) {}
    ValueType distSqr(const Point3D &other) const {
        ValueType dx = x - other.x;
        ValueType dy = y - other.y;
        ValueType dz = z - other.z;
        return dx * dx + dy * dy + dz * dz;
    }
    bool operator ==(const Point3D &other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

class Point3DComparator {
private:
    Point3D p_;
public:
    explicit Point3DComparator(const Point3D &p = Point3D(0, 0, 0)) : p_(p) {}
    bool operator ()(const Point3D &a, const Point3D &b) const {
        return p_.distSqr(a) < p_.distSqr(b);
    }
};

bool testVectorOfRandomPoints(int vectorSize) {
    using namespace std::chrono;
    std::vector<Point3D> v(vectorSize);
    v[0] = Point3D(testing::rand(true), testing::rand(), testing::rand());
    for (int i = 1; i < vectorSize; ++i) {
        v[i] = Point3D(testing::rand(), testing::rand(), testing::rand());
    }
    std::vector<Point3D> v2(v);
    Point3DComparator cmp(Point3D(testing::rand(), testing::rand(), testing::rand()));
    return compareSorts(v.begin(), v.end(), v2.begin(), v2.end(), cmp, timsort::StdTimSortParams());
}

bool testVariousVectorsOfRandomPoints() {
    const int LENGTHS[] = {50, 100, 1000, 10000, 100000, 1000000};
    const int TESTS_COUNT = sizeof(LENGTHS) / sizeof(LENGTHS[0]);
    for (int i = 0; i < TESTS_COUNT; ++i) {
        std::cout << testing::SUBTEST_HEADER << (i + 1) << ". Length = " << LENGTHS[i] << ":" << std::endl;
        bool ok = testVectorOfRandomPoints(LENGTHS[i]);
        std::cout << (ok ? testing::VERDICT_OK : testing::VERDICT_FAILED) << std::endl;
        if (!ok) {
            return false;
        }
    }
    return true;
}

namespace testing {
    Test tests[] = {
            Test{"Vectors of random values", testVariousVectorsOfRandomInts},
            Test{"Arrays of random values", testVariousArraysOfRandomInts},
            Test{"Partially sorted vectors", testPartiallySortedVectors},
            Test{"Concatenated sorted vectors", testVariousConcatenatedSortedVectors},
            Test{"Vectors of random 3D points (Point3D)", testVariousVectorsOfRandomPoints},
    };
    std::size_t testsCount = sizeof(tests) / sizeof(Test);

    bool doTest(const Test &test) {
        std::cout << test.description << ": " << std::endl;
        bool ok = test.testFunc();
        std::cout << (ok ? testing::VERDICT_OK : testing::VERDICT_FAILED) << std::endl;
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