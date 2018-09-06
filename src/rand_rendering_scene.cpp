#include <utility>
#include <random>

#include "rand_rendering_scene.hpp"

RandRenderingScene::RandRenderingScene(RenderingScene &&renderingScene):
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

void RandRenderingScene::progressDo() {
    if (progressWork() < progressGoal()) {
        m_renderingScene.renderRow(m_shuffledRows[m_row++]);
    }
}

size_t RandRenderingScene::progressWork() const {
    return m_row;
}

size_t RandRenderingScene::progressGoal() const {
    return m_renderingScene.height();
}
