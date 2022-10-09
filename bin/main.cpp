
#include <lib/number.h>
#include <iostream>

int main() {
    uint2022_t a = from_string("3456546783947385647389000000000000000000000000000000000000000000078765467897654356789000000066785678745565000");
    uint2022_t b = from_string("40000000000000000000000000055");
    // uint2022_t num1 = from_string(value1);
    //  uint2022_t num2 = from_string(value2);

    uint2022_t c = a / b;
    std::cout << c;
    return 0;
}
