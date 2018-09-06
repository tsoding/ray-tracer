#ifndef RAND_RENDERING_SCENE_HPP_
#define RAND_RENDERING_SCENE_HPP_

#include <vector>

#include "rendering_scene.hpp"

class RandRenderingScene {
 public:
    explicit RandRenderingScene(RenderingScene &&renderingScene);

    void progressDo();
    size_t progressWork() const;
    size_t progressGoal() const;

 private:
    RenderingScene m_renderingScene;
    size_t m_row;
    std::vector<size_t> m_shuffledRows;
};

#endif  // RAND_RENDERING_SCENE_HPP_
