
#include <lib/number.h>
#include <iostream>

int main() {
    uint2022_t a = from_string("200000000234567899087654678900000000000012");
    uint2022_t b = from_string("1197865411111111");

    uint2022_t c = a / b;
    std::cout << c;
    return 0;
}
