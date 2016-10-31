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
        std::function<bool(void)> testFunc;
    };

    extern Test tests[];
    extern std::size_t testsCount;

    bool doTest(const Test &test);
    int rand(bool reset = false);

    const std::string VERDICT_OK = "\033[32;1mOK\033[0m";
    const std::string VERDICT_FAILED = "\033[31;1mFailed\033[0m";
    const std::string TEST_HEADER = "\033[33;1mTest\033[0m #";
    const std::string SUBTEST_HEADER = "\033[36;1mSubtest\033[0m #";

}

#endif //TIMSORT_TESTS_H
