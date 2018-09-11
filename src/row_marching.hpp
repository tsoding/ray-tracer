#ifndef ROW_MARCHING_HPP_
#define ROW_MARCHING_HPP_

#include <cassert>
#include <vector>

#include "scene.hpp"
#include "display.hpp"

template <typename Display>
class RowMarching {
 public:
    RowMarching(const Scene *scene,
                Display *display):
        m_scene(scene),
        m_display(display) {
        assert(scene);
        assert(display);
    }

    void renderRow(size_t row) {
        const float half_width = static_cast<float>(m_display->width()) * 0.5f;
        const float half_height = static_cast<float>(m_display->height()) * 0.5f;

        for (size_t col = 0; col < m_display->width(); ++col) {
            const vec3<float> p = { static_cast<float>(col) - half_width,
                                    static_cast<float>(row) - half_height,
                                    0.0f };

            m_display->put(row, col,
                           march(static_cast<float>(col) - half_width,
                                 static_cast<float>(row) - half_height,
                                 *m_scene,
                                 normalize(p - m_scene->eye)));
        }
    }

    inline size_t height() const {
        return m_display->height();
    }

 private:
    const Scene * const m_scene;
    Display * const m_display;
};

// \brief RowMarching ctor with template param inference
template <typename Display>
inline RowMarching<Display>
mkRowMarching(const Scene *scene, Display *display) {
    return RowMarching<Display>(scene, display);
}

#endif  // ROW_MARCHING_HPP_
