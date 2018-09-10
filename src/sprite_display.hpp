#ifndef SPRITE_TEXTURE_HPP_
#define SPRITE_TEXTURE_HPP_

#include "./texture_display.hpp"

class SpriteDisplay {
 public:
    SpriteDisplay(size_t width, size_t height);

    inline size_t width() const {
        return m_textureDisplay.width();
    }

    inline size_t height() const {
        return m_textureDisplay.height();
    }

    inline void put(size_t row, size_t col, const color &c) {
        return m_textureDisplay.put(row, col, c);
    }

    inline void clean() {
        return m_textureDisplay.clean();
    }

    const sf::Sprite &sprite();

 private:
    TextureDisplay m_textureDisplay;
    sf::Sprite m_sprite;
};

#endif  // SPRITE_TEXTURE_HPP_
