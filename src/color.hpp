#ifndef SRC_COLOR_HPP_
#define SRC_COLOR_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <optional>             // NOLINT

#include "./vec.hpp"

using color = vec<float, 3>;

int digit_hex_to_dec(char c) {
    if ('0' <= c && c <= '9') {
        return c - '0';
    }

    if ('A' <= c && c <= 'F') {
        return c - 'A' + 10;
    }

    return 0;
}

bool is_hex_digit(char c) {
    return ('0' <= c && c <= '9') || ('A' <= c && c <= 'F');
}

bool is_color_hex_code(const std::string &str) {
    if (str.size() != 7) {
        return false;
    }

    if (str[0] != '#') {
        return false;
    }

    for (size_t i = 1; i < str.size(); ++i) {
        if (!is_hex_digit(str[i])) {
            return false;
        }
    }

    return true;
}

/*
 * Easy way out, return as ptr.
 * Return nullptr if invalid str, else return valid color*.
 *
 * TODO: Decide to return as raw ptr, or unique_ptr.
 * Do we want to enforce the user to use smart pointer?
 * unique_ptr is an attractive option, cause it easily converts to shared_ptr
 */
std::unique_ptr<color> color_from_hex(const std::string &str) {
    if (!is_color_hex_code(str)) {
        return nullptr;
    }

    std::array<int, 6> codes;
    std::transform(str.begin() + 1, str.end(), codes.begin(), digit_hex_to_dec);

    const color hex_color = {
        static_cast<float>(codes[0] * 16 + codes[1]) / 255.0f,
        static_cast<float>(codes[2] * 16 + codes[3]) / 255.0f,
        static_cast<float>(codes[4] * 16 + codes[5]) / 255.0f
    };

    return std::make_unique<color>(hex_color);
}

#endif  // SRC_COLOR_HPP_
