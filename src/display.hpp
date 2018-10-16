#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

#include <string>
#include <memory>

#include "color.hpp"

class Display {
 public:
    Display(size_t width, size_t height);
    Display(const Display &display);
    Display(Display &&display);
    ~Display();

    size_t width() const;
    size_t height() const;

    void put(size_t row, size_t col, const Color &c);
    Color pixel(size_t row, size_t col) const;

    void save_as_ppm(const std::string &filename) const;

 private:
    const size_t m_width;
    const size_t m_height;
    Color *m_data;
};

#endif  // DISPLAY_HPP_
