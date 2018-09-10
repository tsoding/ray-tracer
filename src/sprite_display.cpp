#include "sprite_display.hpp"

SpriteDisplay::SpriteDisplay(size_t width, size_t height):
    m_textureDisplay(width, height),
    m_sprite(m_textureDisplay.texture(),
             sf::IntRect(0, 0,
                         static_cast<int>(m_textureDisplay.width()),
                         static_cast<int>(m_textureDisplay.height()))) {
}

const sf::Sprite &SpriteDisplay::sprite() {
    m_sprite.setTexture(m_textureDisplay.texture());
    return m_sprite;
}
