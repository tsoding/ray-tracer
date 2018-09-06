#include "texture_display.hpp"

TextureDisplay::TextureDisplay(size_t width, size_t height):
    m_width(width),
    m_height(height),
    m_buffer(width * height * 4) {
    m_texture.create(static_cast<unsigned int>(width),
                     static_cast<unsigned int>(height));
}

size_t TextureDisplay::width() const {
    return m_width;
}

size_t TextureDisplay::height() const {
    return m_height;
}

void TextureDisplay::put(size_t row, size_t col, const color &c) {
    m_buffer[row * m_width * 4 + col * 4 + 0] = static_cast<sf::Uint8>(c.v[0] * 255.0f);
    m_buffer[row * m_width * 4 + col * 4 + 1] = static_cast<sf::Uint8>(c.v[1] * 255.0f);
    m_buffer[row * m_width * 4 + col * 4 + 2] = static_cast<sf::Uint8>(c.v[2] * 255.0f);
    m_buffer[row * m_width * 4 + col * 4 + 3] = 255;
}

sf::Texture TextureDisplay::as_texture() {
    m_texture.update(m_buffer.data());
    return m_texture;
}
