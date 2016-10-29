#include <utils.h>

namespace timsort {
    std::size_t getMinrun(std::size_t length) {
        std::size_t r = 0;
        while (length >= 64) {
            r |= length & 1;
            length >>= 1;
        }
        return length + r;
    }

    double abs(double x) {
        return x < 0 ? -x : x;
    }

    unsigned long long sqrt(unsigned long long n) {
        double a = 1e6; // Just a random initial value
        while (abs(a * a - n) > 1) {
            a = (a + n / a) / 2;
        }
        unsigned long long ans = a + 3;
        while (ans * ans > n) {
            --ans;
        }
        return ans;
    }
}
