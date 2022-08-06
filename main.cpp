#include <iostream>
#include "minmax.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    MinMax minMax = MinMax();
    int res = minMax.max(0, 0);
    std::cout << res << std::endl;
    return 0;
}
