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
    static const size_t kArraySize = 68;
    static const uint32_t kInf = 4294967295;

    uint2022_t();
    uint2022_t(uint32_t value);
    uint2022_t(const char* buff);

    void RemoveLeadingZeros();
    void Extend(size_t new_size);
    size_t GetNumberSize() const;

    uint32_t digits[kArraySize];
};

static_assert(sizeof(uint2022_t) <= 300, "Size of uint2022_t must be no higher than 300 bytes");

uint2022_t from_uint(uint32_t i);
uint2022_t from_string(const char* buff);

void Copy(uint32_t* from, uint32_t* to, size_t size);

uint2022_t BitwiseShift(const uint2022_t& number, uint32_t pow);
std::pair <uint32_t, uint2022_t> DivideBinary(const uint2022_t& lhs, const uint2022_t& rhs);

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


