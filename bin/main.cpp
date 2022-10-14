
#include <lib/number.h>
#include <iostream>

int main() {
    uint2022_t a = from_string("999999999999999999");
    uint2022_t b = from_string("111111111111111111");

    uint2022_t c = a / b;
    std::cout << c;
    return 0;
}
