#ifndef SEQ_RENDERING_SCENE_HPP_
#define SEQ_RENDERING_SCENE_HPP_

#include <utility>

#include "rendering_scene.hpp"

template <typename Display>
class SeqRenderingScene {
 public:
    explicit SeqRenderingScene(RowMarching<Display> &&renderingScene):
        m_renderingScene(std::move(renderingScene)) {
    }

    void progressDo() {
        if (progressWork() < progressGoal()) {
            m_renderingScene.renderRow(m_row++);
        }
    }

    size_t progressWork() const {
        return m_row;
    }

    size_t progressGoal() const {
        return m_renderingScene.height();
    }

    void progressReset() {
        m_row = 0;
    }

 private:
    RowMarching<Display> m_renderingScene;
    size_t m_row;
};

template <typename Display>
inline SeqRenderingScene<Display>
mkSeqRenderingScene(RowMarching<Display> &&renderingScene) {
    return SeqRenderingScene<Display>(std::move(renderingScene));
}

#endif  // SEQ_RENDERING_SCENE_HPP_
