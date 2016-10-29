//
// Created by xenon on 10/30/16.
//

#include <debug.h>
#include <list.h>
#include <iostream>

using std::cout;
using std::endl;
using timsort::List;

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
    PRINTABLE(Point);
};

PRINTABLE_BEGIN(Point)
    FIELD(x)
    FIELD(y)
PRINTABLE_END

int main() {
    List<Point> list;
    for (int i = 0; i < 10; ++i) {
        list.pushBack(Point(i, 20 - i));
    }

    for (auto it = list.begin(); it != list.end(); ++it) {
        if (it->x % 2 == 0) {
            list.erase(it);
        }
    }

    cout << list.size() << endl;
    cout << list << endl;
    DELIM;
    cout << std::boolalpha << is_iterator<List<int>::iterator>::value << endl;
    return 0;
}
