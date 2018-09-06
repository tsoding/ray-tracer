#ifndef RENDERINGSCENE_HPP_
#define RENDERINGSCENE_HPP_

#include <vector>

#include "scene.hpp"
#include "display.hpp"

class RenderingScene {
 public:
    RenderingScene(const Scene *scene,
                   Display *display);

    void renderRow(size_t row);

 private:
    const Scene * const m_scene;
    Display * const m_display;
};

#endif  // RENDERINGSCENE_HPP_