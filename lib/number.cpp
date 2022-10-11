#include "number.h"

uint2022_t::uint2022_t() {
    for (size_t i = 0; i < uint2022_t::kArraySize; ++i) {
        digits[i] = uint2022_t::kInf;
    }
}

uint2022_t from_uint(uint32_t i) {
    return uint2022_t(i);
}

uint2022_t from_string(const char *buff) {
    return uint2022_t(buff);
}


uint2022_t Pow(const uint2022_t &lhs,
               uint32_t pow) { // lhs * (10 ^ pow), where pow can only be 18 or 9, due to Karatsuba algorithm
    uint2022_t result;
    size_t size = lhs.GetNumberSize();
    size_t i;
    for (i = 0; i < pow / 9; ++i) {
        result.digits[i] = 0;
    }
    for (size_t j = 0; j < size; ++j, ++i) {
        result.digits[i] = lhs.digits[j];
    }
    result.RemoveLeadingZeros();
    return result;
}

std::pair<uint32_t, uint2022_t> DivideBinary(uint2022_t lhs, uint2022_t rhs) {
    uint32_t quotient = 0;

    uint32_t left = 0;
    uint32_t right = uint2022_t::kBase;

    while (right - 1 > left) {
        uint32_t mid = (right + left) / 2;

        uint2022_t product = uint2022_t(mid) * rhs;
        if (product > lhs) {
            right = mid;
        } else {
            left = mid;
        }
    }

    quotient = left;
    uint2022_t tmp = (quotient * rhs);
    uint2022_t remainder = lhs - tmp;
    return std::make_pair(quotient, remainder);
}

void Copy(uint32_t *from, uint32_t *to, size_t size) {
    while (size > 0) {
        *to = *from;
        ++to;
        ++from;
        --size;
    }
}



void uint2022_t::Extend(size_t new_size) {
    size_t cur_size = GetNumberSize();

    for (size_t i = cur_size; i < new_size; ++i) {
        digits[i] = 0;
    }
}

size_t uint2022_t::GetNumberSize() const {
    size_t size = 0;
    for (size_t i = uint2022_t::kArraySize - 1; i-- > 0;) {
        if (digits[i] != uint2022_t::kInf) {
            break;
        } else {
            ++size;
        }
    }
    return uint2022_t::kArraySize - size;
}

void uint2022_t::RemoveLeadingZeros() {
    for (size_t i = uint2022_t::kArraySize - 1; i-- > 0;) {
        if (digits[i] == uint2022_t::kInf) {
            continue;
        } else if (digits[i] == 0) {
            digits[i] = uint2022_t::kInf;
        } else {
            break;
        }
    }
}


uint2022_t operator+(const uint2022_t &lhs, const uint2022_t &rhs) {
    uint2022_t result;
    uint32_t carry = 0;

    size_t lhs_size = lhs.GetNumberSize();
    size_t rhs_size = rhs.GetNumberSize();
    size_t i;
    for (i = 0; i < std::max(lhs_size, rhs_size); ++i) {
        uint32_t place_sum = carry;
        if (i < lhs_size) {
            place_sum += lhs.digits[i];
        }
        if (i < rhs_size) {
            place_sum += rhs.digits[i];
        }

        if (place_sum > uint2022_t::kBase) {
            carry = place_sum / uint2022_t::kBase;
            place_sum = place_sum % uint2022_t::kBase;
        } else {
            carry = 0;
        }
        result.digits[i] = place_sum;
    }

    if (carry != 0 && i < uint2022_t::kArraySize) {
        result.digits[i] = carry;
    }
    return result;
}

uint2022_t operator-(const uint2022_t &lhs, const uint2022_t &rhs) {
    if (lhs < rhs) {
        return uint2022_t(-1); // Undefined Behavior
    }

    uint2022_t result;
    uint32_t carry = 0;
    size_t rhs_size = rhs.GetNumberSize();
    size_t lhs_size = lhs.GetNumberSize();

    for (size_t i = 0; i < lhs_size; ++i) {
        uint32_t result_num = 0;
        uint32_t left_num = lhs.digits[i];
        uint32_t right_num = 0;
        if (i < rhs_size) {
            right_num = rhs.digits[i];
        }

        if (left_num > right_num || (left_num == right_num && carry == 0)) {
            result_num = left_num - right_num - carry;
            carry = 0;
        } else {
            result_num = left_num - right_num - carry + uint2022_t::kBase;
            carry = 1;
        }
        result.digits[i] = result_num;
    }

    result.RemoveLeadingZeros();
    return result;
}

uint2022_t operator*(const uint2022_t &lhs, const uint2022_t &rhs) {
    if (lhs.GetNumberSize() == 1 && rhs.GetNumberSize() == 1) {
        uint64_t res = static_cast<uint64_t> (lhs.digits[0]) * rhs.digits[0];
        std::string str = std::to_string(res);
        return uint2022_t{str.c_str()};
    }

    uint2022_t x = lhs;
    uint2022_t y = rhs;

    size_t max_size = std::max(lhs.GetNumberSize(), rhs.GetNumberSize());
    x.Extend(max_size);
    y.Extend(max_size);


    size_t mid = max_size / 2;
    uint2022_t x_left, x_right, y_left, y_right;

    Copy(x.digits + mid, x_right.digits, max_size - mid);
    Copy(x.digits, x_left.digits, mid);
    Copy(y.digits + mid, y_right.digits, max_size - mid);
    Copy(y.digits, y_left.digits, mid);

    uint2022_t first = (x_left * y_left);
    uint2022_t second = (x_right * y_right);
    uint2022_t third = (x_left + x_right) * (y_left + y_right);

    uint2022_t tmp1 = Pow(second, mid * 18);
    uint2022_t tmp4 = third - first;
    uint2022_t tmp3 = tmp4 - second;
    uint2022_t tmp2 = Pow(tmp3, mid * 9);


    uint2022_t result = tmp1 +
                        tmp2 + first;
    result.RemoveLeadingZeros();
    return result;
}

uint2022_t operator/(const uint2022_t &lhs, const uint2022_t &rhs) {
    uint2022_t ans;
    size_t ans_size = 0;

    uint2022_t rem(uint32_t{0});

    int i = lhs.GetNumberSize() - 1;
    while (i >= 0) {
        uint2022_t cur_lhs;
        if (rem != 0) {
            Copy(rem.digits, cur_lhs.digits, rem.GetNumberSize());
        } else {
            if (lhs.digits[i] == 0) {
                ans.digits[ans_size++] = 0;
                --i;
                continue;
            }
        }

        while (cur_lhs < rhs && i >= 0) {
            size_t tmp_size = cur_lhs.GetNumberSize();
            for (int j = tmp_size; j > 0; --j) {
                cur_lhs.digits[j] = cur_lhs.digits[j - 1];
            }
            cur_lhs.digits[0] = lhs.digits[i];
            --i;
            if (cur_lhs < rhs) {
                ans.digits[ans_size++] = 0;
            }
        }

        if (cur_lhs > rhs) {
            std::pair<uint32_t, uint2022_t> tmp = DivideBinary(cur_lhs, rhs);
            rem = tmp.second;
            ans.digits[ans_size++] = tmp.first;
        }
    }
    std::reverse(ans.digits, ans.digits + ans.GetNumberSize());
    ans.RemoveLeadingZeros();
    return ans;
}

bool operator==(const uint2022_t &lhs, const uint2022_t &rhs) {
    for (size_t i = 0; i < uint2022_t::kArraySize; ++i) {
        if (lhs.digits[i] != rhs.digits[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const uint2022_t &lhs, const uint2022_t &rhs) {
    return !(lhs == rhs);
}

bool operator==(const uint2022_t &lhs, const uint32_t &rhs) {
    if (rhs % uint2022_t::kBase != lhs.digits[0]) {
        return false;
    }

    if (rhs / uint2022_t::kBase > 0) {
        return rhs / uint2022_t::kBase == lhs.digits[1];
    }

    return true;
}

bool operator!=(const uint2022_t &lhs, const uint32_t &rhs) {
    return !(lhs == rhs);
}

bool operator>(const uint2022_t &lhs, const uint2022_t &rhs) {
    size_t lhs_size = lhs.GetNumberSize();
    size_t rhs_size = rhs.GetNumberSize();

    if (lhs_size > rhs_size) {
        return true;
    } else if (lhs_size < rhs_size) {
        return false;
    }
    for (size_t i = lhs_size - 1; i-- > 0;) {
        if (lhs.digits[i] == rhs.digits[i]) {
            continue;
        }

        return lhs.digits[i] > rhs.digits[i];
    }
    return false;
}

bool operator<(const uint2022_t &lhs, const uint2022_t &rhs) {
    return rhs > lhs;
}

bool operator>=(const uint2022_t &lhs, const uint2022_t &rhs) {
    for (size_t i = uint2022_t::kArraySize - 1; i-- > 0;) {
        if (lhs.digits[i] == rhs.digits[i]) {
            continue;
        }
        if (lhs.digits[i] == uint2022_t::kInf) {
            return false;
        } else if (rhs.digits[i] == uint2022_t::kInf) {
            return true;
        }
        return lhs.digits[i] >= rhs.digits[i];
    }
    return false;
}

bool operator<=(const uint2022_t &lhs, const uint2022_t &rhs) {
    return rhs >= lhs;
}


std::ostream &operator<<(std::ostream &stream, const uint2022_t &value) {
    stream << value.digits[value.GetNumberSize() - 1];
    for (size_t i = value.GetNumberSize() - 2; i-- > 0;) {
        if (value.digits[i] == 0) {
            stream << "000000000";
            continue;
        }
        uint32_t tmp = value.digits[i];
        while (tmp < 1e8) {
            stream << 0;
            tmp *= 10;
        }
        stream << value.digits[i];
    }
    return stream;
}


uint2022_t::uint2022_t(const char *buff) {
    size_t i;
    size_t ind = 0;
    uint32_t cur_place = 0;
    for (i = strlen(buff); i >= 9; i -= 9) {
        cur_place = 0;
        for (size_t j = i - 9; j < i; ++j) {
            cur_place = (cur_place * 10) +
                        (static_cast <int> (buff[j]) - static_cast <int> ('0'));
        }
        digits[ind++] = cur_place;
    }
    if (i != 0) {
        cur_place = 0;
        for (size_t j = 0; j < i; ++j) {
            cur_place = (cur_place * 10) +
                        (static_cast <int> (buff[j]) - static_cast <int> ('0'));
        }
        digits[ind++] = cur_place;
    }

    for (; ind < uint2022_t::kArraySize; ++ind) {
        digits[ind] = uint2022_t::kInf;
    }
}

uint2022_t::uint2022_t(uint32_t value) {
    size_t i = 0;
    digits[i++] = value % uint2022_t::kBase;
    if (value / uint2022_t::kBase > 0) {
        digits[i++] = value / uint2022_t::kBase;
    }

    for (; i < uint2022_t::kArraySize; ++i) {
        digits[i] = uint2022_t::kInf;
    }
}

