#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

#include <string>
#include <memory>

#include "color.hpp"

struct Display {
    Display(size_t width, size_t height);
    Display(const Display &display);
    Display(Display &&display);
    ~Display();

    void put(size_t row, size_t col, const Color &c);
    Color pixel(size_t row, size_t col) const;

    const size_t width;
    const size_t height;
    Color *data;
};

void save_as_ppm(const Display &display,
                 const std::string &filename);

#endif  // DISPLAY_HPP_
