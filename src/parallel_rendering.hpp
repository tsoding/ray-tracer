#ifndef PARALLEL_RENDERING_HPP_
#define PARALLEL_RENDERING_HPP_

#include <thread>
#include <vector>

#include "row_marching.hpp"

template <typename RowRendering>
class ParallelRendering {
public:
    ParallelRendering(RowRendering &&renderingScene,
                           size_t poolSize):
        m_renderingScene(std::move(renderingScene)),
        m_threadPool(poolSize),
        m_row(0)
    {}

    inline void progressDo() {
        if (progressWork() >= progressGoal()) {
            return;
        }

        for (size_t i = 0; i < m_threadPool.size() && m_row < m_renderingScene.height(); ++i, ++m_row) {
            m_threadPool[i] = std::thread([&] () {
                m_renderingScene.renderRow(m_row);
            });
        }

        for (size_t i = 0; i < m_threadPool.size(); ++i) {
            m_threadPool[i].join();
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
    std::vector<std::thread> m_threadPool;
    size_t m_row;
};

template <typename RowRendering>
inline ParallelRendering<RowRendering>
mkParallelRendering(RowRendering &&renderingScene,
                         size_t poolSize) {
    return ParallelRendering<RowRendering>(std::move(renderingScene), poolSize);
}

#endif  // PARALLEL_RENDERING_HPP_
