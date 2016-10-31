//
// Created by xenon on 10/30/16.
//

#include <tests.h>
#include <iostream>
#include <string>
#include <set>

int main(int argc, char *argv[]) {
    std::set<int> requiredTests;
    bool allTests = true;
    for (int i = 1; i < argc; ++i) {
        allTests = false;
        requiredTests.insert(atoi(argv[i]));
    }
    for (int i = 1; i <= testing::testsCount; ++i) {
        if (!allTests && requiredTests.find(i) == requiredTests.end()) {
            continue;
        }
        std::cout << testing::TEST_HEADER << i << ". ";
        if (!testing::doTest(testing::tests[i - 1])) {
            return i;
        }
    }
    return 0;
}
