#include <cassert>

#include "rendering_scene.hpp"

RenderingScene::RenderingScene(const Scene *scene,
                               Display *display):
    m_scene(scene),
    m_display(display) {
    assert(scene);
    assert(display);
}

void RenderingScene::renderRow(size_t row) {
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
