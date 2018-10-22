#include <algorithm>
#include <fstream>
#include <string>

#include "display.hpp"

Display::Display(size_t width, size_t height):
    width(width),
    height(height),
    data(new Color[width * height]) {
}

Display::Display(const Display &display):
    width(display.width),
    height(display.height),
    data(new Color[width * height]) {
    std::copy(display.data,
              display.data + width * height,
              data);
}

Display::Display(Display &&display):
    width(display.width),
    height(display.height),
    data(display.data) {
}

Display::~Display() {
    delete[] data;
}

void Display::put(size_t row, size_t col, const Color &c) {
    data[row * width + col] = c;
}

Color Display::pixel(size_t row, size_t col) const {
    return data[row * width + col];
}

void save_as_ppm(const Display &display,
                 const std::string &filepath) {
    std::ofstream fout(
        filepath,
        std::ofstream::out | std::ostream::binary);

    fout << "P6" << std::endl;
    fout << display.width << " " << display.height << std::endl;
    fout << 255 << std::endl;

    for (size_t row = 0; row < display.height; ++row) {
        for (size_t col = 0; col < display.width; ++col) {
            for (size_t k = 0; k < 3; ++k) {
                const float x = 255.0f * display.pixel(row, col).v[k];
                fout.put(static_cast<char>(x));
            }
        }
    }
}
