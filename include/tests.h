//
// Created by xenon on 10/30/16.
//

#ifndef TIMSORT_TESTS_H
#define TIMSORT_TESTS_H

#include <functional>
#include <string>

namespace testing {
    struct Test {
        std::string description;
        std::function<bool()> testFunc;
    };

    extern Test tests[];
    extern std::size_t testsCount;

    bool doTest(const Test &test);
}

#endif //TIMSORT_TESTS_H
