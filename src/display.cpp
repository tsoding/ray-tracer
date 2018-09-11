#include <algorithm>
#include <fstream>
#include <string>

#include "display.hpp"

Display::Display(size_t width, size_t height):
    m_width(width),
    m_height(height),
    m_data(new color[width * height]) {
}

Display::Display(const Display &display):
    m_width(display.m_width),
    m_height(display.m_height),
    m_data(new color[m_width * m_height]) {
    std::copy(display.m_data,
              display.m_data + m_width * m_height,
              m_data);
}

Display::Display(Display &&display):
    m_width(display.m_width),
    m_height(display.m_height),
    m_data(display.m_data) {
}

Display::~Display() {
    delete[] m_data;
}

size_t Display::width() const {
    return m_width;
}

size_t Display::height() const {
    return m_height;
}

void Display::put(size_t row, size_t col, const color &c) {
    m_data[row * m_width + col] = c;
}

color Display::pixel(size_t row, size_t col) const {
    return m_data[row * m_width + col];
}

void Display::save_as_ppm(const std::string &filepath) const {
    std::ofstream fout(filepath, std::ofstream::out | std::ostream::binary);

    fout << "P6" << std::endl;
    fout << m_width << " " << m_height << std::endl;
    fout << 255 << std::endl;

    for (size_t row = 0; row < m_height; ++row) {
        for (size_t col = 0; col < m_width; ++col) {
            for (size_t k = 0; k < 3; ++k) {
                const float x = 255.0f * m_data[row * m_width + col].v[k];
                fout.put(static_cast<char>(x));
            }
        }
    }
}
