/**
 * @file utils.hpp
 * @author Diego Andrade (bets636@gmail.com)
 * @brief Collection of useful functions and const
 * @version 0.1
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <array>

std::array<char, 4> to_hexstring(const char& byte) {
    std::array<char, 4> decoded{'0', 'x'};

    int upper = byte >> 4;
    decoded[2] = (upper > 9) ? 'A' + upper - 10 : '0' + upper;

    int lower = byte & 0x0f;
    decoded[3] = (lower > 9) ? 'A' + lower - 10 : '0' + lower;
    return decoded;
}

