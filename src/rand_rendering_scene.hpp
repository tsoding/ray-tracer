#ifndef RAND_RENDERING_SCENE_HPP_
#define RAND_RENDERING_SCENE_HPP_

#include <vector>
#include <utility>

template <typename RowRendering>
class RandRenderingScene {
 public:
    explicit RandRenderingScene(RowRendering &&renderingScene):
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
    RowRendering m_renderingScene;
    size_t m_row;
    std::vector<size_t> m_shuffledRows;
};

template <typename RowRendering>
inline RandRenderingScene<RowRendering>
mkRandRenderingScene(RowRendering &&renderingScene) {
    return RandRenderingScene<RowRendering>(std::move(renderingScene));
}

#endif  // RAND_RENDERING_SCENE_HPP_
