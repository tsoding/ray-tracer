#ifndef SRC_COLOR_HPP_
#define SRC_COLOR_HPP_

#include <memory>
#include <string>

#include "./vec.hpp"

using color = vec<float, 3>;

std::unique_ptr<color> color_from_hex(const std::string &str);

#endif  // SRC_COLOR_HPP_
