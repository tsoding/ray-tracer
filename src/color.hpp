#ifndef SRC_COLOR_HPP_
#define SRC_COLOR_HPP_

#include <algorithm>
#include <string>
#include <optional>             // NOLINT

#include "./vec.hpp"

using color = vec<float, 3>;

std::optional<color> color_from_hex(const std::string &str);

#endif  // SRC_COLOR_HPP_
