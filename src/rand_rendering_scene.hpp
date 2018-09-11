#ifndef RAND_RENDERING_SCENE_HPP_
#define RAND_RENDERING_SCENE_HPP_

#include <vector>
#include <utility>

#include "rendering_scene.hpp"

template <typename Display>
class RandRenderingScene {
 public:
    explicit RandRenderingScene(RowMarching<Display> &&renderingScene):
        m_renderingScene(std::move(renderingScene)),
        m_row(0),
        m_shuffledRows(renderingScene.height()) {
        std::mt19937 gen { std::random_device {}() };
        std::iota(m_shuffledRows.begin(),
                  m_shuffledRows.end(),
                  0);
        std::shuffle(m_shuffledRows.begin(),
                     m_shuffledRows.end(),
                     gen);
    }


    void progressDo() {
        if (progressWork() < progressGoal()) {
            m_renderingScene.renderRow(m_shuffledRows[m_row++]);
        }
    }

    inline size_t progressWork() const {
        return m_row;
    }

    inline size_t progressGoal() const {
        return m_renderingScene.height();
    }

    inline void progressReset() {
        m_row = 0;
    }

 private:
    RowMarching<Display> m_renderingScene;
    size_t m_row;
    std::vector<size_t> m_shuffledRows;
};

template <typename Display>
inline RandRenderingScene<Display>
mkRandRenderingScene(RowMarching<Display> &&renderingScene) {
    return RandRenderingScene<Display>(std::move(renderingScene));
}

#endif  // RAND_RENDERING_SCENE_HPP_
