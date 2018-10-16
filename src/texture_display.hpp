#ifndef TEXTURE_DISPLAY_HPP_
#define TEXTURE_DISPLAY_HPP_

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>    // NOLINT

#include "color.hpp"

class TextureDisplay {
 public:
    TextureDisplay(size_t width, size_t height);

    size_t width() const;
    size_t height() const;

    void put(size_t row, size_t col, const Color &c);
    void clean();

    const sf::Texture &texture();

 private:
    const size_t m_width;
    const size_t m_height;
    std::unique_ptr<sf::Uint8[]> m_buffer;
    sf::Texture m_texture;
};

#endif  // TEXTURE_DISPLAY_HPP_
