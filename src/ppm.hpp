#ifndef PPM_HPP_
#define PPM_HPP_

#include <string>

void save_ppm_file(const color *display,
                   size_t width,
                   size_t height,
                   const std::string &filepath);

#endif  // PPM_HPP_
