#ifndef SEQ_RENDERING_SCENE_HPP_
#define SEQ_RENDERING_SCENE_HPP_

#include "rendering_scene.hpp"

class SeqRenderingScene {
 public:
    explicit SeqRenderingScene(RenderingScene &&renderingScene);

    void progressDo();
    size_t progressWork() const;
    size_t progressGoal() const;

 private:
    RenderingScene m_renderingScene;
    size_t m_row;
};

#endif  // SEQ_RENDERING_SCENE_HPP_
