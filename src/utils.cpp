#include <utils.h>

namespace timsort {
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
