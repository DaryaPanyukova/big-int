#pragma once
#include <cinttypes>
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <stdint.h>
#include <iomanip>

struct uint2022_t {
    static const uint32_t kBase = 1e9;
   // static const size_t SIZE = 68;

    uint2022_t() = default;
    uint2022_t(uint32_t value);
    uint2022_t(const char* buff);

    void RemoveLeadingZeros();
    void Extend(size_t new_size);


    std::vector <uint32_t> number;
};

static_assert(sizeof(uint2022_t) <= 300, "Size of uint2022_t must be no higher than 300 bytes");

uint2022_t from_uint(uint32_t i);
uint2022_t from_string(const char* buff);

uint2022_t Pow(const uint2022_t& number, uint32_t pow);
std::pair <uint32_t, uint2022_t> DivideBinary(uint2022_t lhs, uint2022_t rhs);

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs);
uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs);
uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs);
uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs);
bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator==(const uint2022_t& lhs, const uint32_t& rhs);
bool operator!=(const uint2022_t& lhs, const uint32_t& rhs);

bool operator>(const uint2022_t& lhs, const uint2022_t& rhs);
bool operator<(const uint2022_t& lhs, const uint2022_t& rhs);
bool operator<=(const uint2022_t& lhs, const uint2022_t& rhs);
bool operator>=(const uint2022_t& lhs, const uint2022_t& rhs);

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value);


