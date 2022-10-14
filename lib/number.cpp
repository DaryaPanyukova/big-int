#include "number.h"


uint2022_t from_uint(uint32_t i) {
    return uint2022_t(i);
}

uint2022_t from_string(const char *buff) {
    return uint2022_t(buff);
}


void uint2022_t::Extend(size_t new_size) {
    size_t cur_size = number.size();
    while (cur_size < new_size) {
        number.push_back(0);
        cur_size++;
    }
}

void uint2022_t::RemoveLeadingZeros() {
    while (number.size() > 1 && number[number.size() - 1] == 0) {
        number.pop_back();
    }
    if (number.size() == 0) {
        number.push_back(0);
    }
}


uint2022_t operator+(const uint2022_t &lhs, const uint2022_t &rhs) {
    uint2022_t result;
    uint32_t carry = 0;

    if (lhs.number.size() > rhs.number.size()) {
        for (size_t i = 0; i < lhs.number.size(); ++i) {
            uint32_t place_sum = lhs.number[i] + carry;
            if (i < rhs.number.size()) {
                place_sum += rhs.number[i];
            }
            if (place_sum > uint2022_t::kBase) {
                carry = place_sum / uint2022_t::kBase;
                place_sum = place_sum % uint2022_t::kBase;
            } else {
                carry = 0;
            }
            result.number.push_back(place_sum);
        }
    } else {
        for (size_t i = 0; i < rhs.number.size(); ++i) {
            uint32_t place_sum = rhs.number[i] + carry;
            if (i < lhs.number.size()) {
                place_sum += lhs.number[i];
            }
            if (place_sum > uint2022_t::kBase) {
                carry = place_sum / uint2022_t::kBase;
                place_sum = place_sum % uint2022_t::kBase;
            } else {
                carry = 0;
            }
            result.number.push_back(place_sum);
        }
    }

    if (carry != 0) {
        result.number.push_back(carry);
    }
    return result;
}

uint2022_t operator-(const uint2022_t &lhs, const uint2022_t &rhs) {
    if (lhs < rhs) {
        return uint2022_t(-1); // Undefined Behavior
    }

    uint2022_t result;
    uint32_t carry = 0;
    for (size_t i = 0; i < lhs.number.size(); ++i) {
        uint32_t result_num = 0;
        uint32_t left_num = lhs.number[i];
        uint32_t right_num;
        if (i < rhs.number.size()) {
            right_num = rhs.number[i];
        } else {
            right_num = 0;
        }

        if (left_num > right_num || (left_num == right_num && carry == 0)) {
            result_num = left_num - right_num - carry;
            carry = 0;
        } else {
            result_num = left_num - right_num - carry + uint2022_t::kBase;
            carry = 1;
        }
        result.number.push_back(result_num);
    }
    result.RemoveLeadingZeros();
    return result;
}


uint2022_t operator*(const uint2022_t &lhs, const uint2022_t &rhs) {
    if (lhs.number.size() == 1 && rhs.number.size() == 1) {
        uint64_t res = static_cast<uint64_t> (lhs.number[0]) * rhs.number[0];
        std::string str = std::to_string(res);
        return uint2022_t{str.c_str()};
    }

    uint2022_t x = lhs;
    uint2022_t y = rhs;

    size_t max_size = std::max(lhs.number.size(), rhs.number.size());
    x.Extend(max_size);
    y.Extend(max_size);


    size_t mid = max_size / 2;
    uint2022_t x_left, x_right, y_left, y_right;

    x_right.number = std::vector<uint32_t>(x.number.begin() + mid,
                                           x.number.end());
    x_left.number = std::vector<uint32_t>(x.number.begin(),
                                          x.number.begin() + mid);

    y_right.number = std::vector<uint32_t>(y.number.begin() + mid,
                                           y.number.end());
    y_left.number = std::vector<uint32_t>(y.number.begin(),
                                          y.number.begin() + mid);

    uint2022_t first = (x_left * y_left);
    uint2022_t second = (x_right * y_right);
    uint2022_t third = (x_left + x_right) * (y_left + y_right);

    uint2022_t result = BitwiseShift(second, mid * 2) +
            BitwiseShift(third - first - second, mid) + first;
    result.RemoveLeadingZeros();
    return result;
}

uint2022_t operator/(const uint2022_t &lhs, const uint2022_t &rhs) {
    uint2022_t ans;
    uint2022_t rem(uint32_t{0});

    int i = lhs.number.size() - 1;
    while (i >= 0) {
        uint2022_t cur_lhs;
        if (rem != 0) {
            cur_lhs.number = rem.number;
        } else {
            if (lhs.number[i] == 0) {
                ans.number.push_back(0);
                --i;
                continue;
            }
        }

        while (cur_lhs < rhs && i >= 0) {
            cur_lhs.number.insert(cur_lhs.number.begin(), lhs.number[i]);
            --i;
        }

        if (cur_lhs < rhs) {
            ans.number.push_back(0);
        } else {
            std::pair<uint32_t, uint2022_t> tmp = DivideBinary(cur_lhs, rhs);
            rem = tmp.second;
            ans.number.push_back(tmp.first);
        }
    }
    std::reverse(ans.number.begin(), ans.number.end());
    return ans;
}


std::pair<uint32_t, uint2022_t>
DivideBinary(const uint2022_t &lhs, const uint2022_t &rhs) {
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
    uint2022_t remainder = lhs - (quotient * rhs);
    return std::make_pair(quotient, remainder);
}

bool operator==(const uint2022_t &lhs, const uint2022_t &rhs) {
    if (lhs.number.size() != rhs.number.size()) {
        return false;
    }
    return lhs.number == rhs.number;
}

bool operator!=(const uint2022_t &lhs, const uint2022_t &rhs) {
    return !(lhs == rhs);
}

bool operator==(const uint2022_t &lhs, const uint32_t &rhs) {
    if (rhs % uint2022_t::kBase != lhs.number[0]) {
        return false;
    }

    if (rhs / uint2022_t::kBase > 0) {
        if (lhs.number.size() < 2) {
            return false;
        }
        if (rhs / uint2022_t::kBase != lhs.number[1]) {
            return false;
        }
    }

    return true;
}

bool operator!=(const uint2022_t &lhs, const uint32_t &rhs) {
    return !(lhs == rhs);
}

bool operator>(const uint2022_t &lhs, const uint2022_t &rhs) {
    return rhs < lhs;
}

bool operator<(const uint2022_t &lhs, const uint2022_t &rhs) {
    if (lhs.number.size() < rhs.number.size()) {
        return true;
    } else if (lhs.number.size() > rhs.number.size()) {
        return false;
    }

    for (int i = lhs.number.size() - 1; i >= 0; --i) {
        if (lhs.number[i] == rhs.number[i]) {
            continue;
        }
        return lhs.number[i] < rhs.number[i];
    }
    return false;
}

bool operator>=(const uint2022_t &lhs, const uint2022_t &rhs) {
    return !(lhs < rhs);
}

bool operator<=(const uint2022_t &lhs, const uint2022_t &rhs) {
    return !(rhs < lhs);
}


std::ostream &operator<<(std::ostream &stream, const uint2022_t &value) {
    stream << value.number[value.number.size() - 1];
    for (int i = value.number.size() - 2; i >= 0; --i) {
        stream << std::setfill('0') << std::setw(9) << value.number[i];
    }
    return stream;
}


uint2022_t::uint2022_t(const char *buff) {
    if (strcmp(buff, "") == 0) {
        this->number.push_back(-1); // Undefined behavior
        return;
    }

    size_t i;
    uint32_t cur_place = 0;
    for (i = strlen(buff); i >= 9; i -= 9) {
        cur_place = 0;
        for (size_t j = i - 9; j < i; ++j) {
            cur_place = (cur_place * 10) + (buff[j] - '0');
        }
        number.push_back(cur_place);
    }
    if (i != 0) {
        cur_place = 0;
        for (size_t j = 0; j < i; ++j) {
            cur_place = (cur_place * 10) + (buff[j] - '0');
        }
        number.push_back(cur_place);
    }
}

uint2022_t::uint2022_t(uint32_t value) {
    number.push_back(value % uint2022_t::kBase);
    if (value / uint2022_t::kBase > 0) {
        number.push_back(value / uint2022_t::kBase);
    }
}

uint2022_t BitwiseShift(const uint2022_t &lhs, uint32_t place) {
    uint2022_t result;
    for (size_t i = 0; i < place; ++i) {
        result.number.push_back(0);
    }
    for (size_t i = 0; i < lhs.number.size(); ++i) {
        result.number.push_back(lhs.number[i]);
    }
    result.RemoveLeadingZeros();
    return result;
}
