//
// Created by xenon on 10/30/16.
//

#include <tests.h>
#include <iostream>

int main() {
    for (int i = 0; i < testing::testsCount; ++i) {
        std::cout << testing::TEST_HEADER << (i + 1) << ". ";
        if (!testing::doTest(testing::tests[i])) {
            return i + 1;
        }
    }
    return 0;
}
