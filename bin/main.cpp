
#include <lib/number.h>
#include <iostream>


int main() {
    uint2022_t a = from_string(
            "456707765876543000000000000000000000000000007000000000000000000000000000000");
    uint2022_t b = from_string("1637654332");

    uint2022_t c = a / b;
    std::cout << c;
    return 0;
}