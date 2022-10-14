
#include <lib/number.h>
#include <iostream>

int main() {
    uint2022_t a = from_string("405272312330606683982498447530407677486444946329741970511324085183808429621437");
    uint2022_t b = from_string("8765436543");

    uint2022_t c = a / b;
    std::cout << c;
    return 0;
}
