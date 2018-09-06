#include <utility>

#include "seq_rendering_scene.hpp"

SeqRenderingScene::SeqRenderingScene(RenderingScene &&renderingScene):
    m_renderingScene(std::move(renderingScene)) {
}

void SeqRenderingScene::progressDo() {
    if (progressWork() < progressGoal()) {
        m_renderingScene.renderRow(m_row++);
    }
}

size_t SeqRenderingScene::progressWork() const {
    return m_row;
}

size_t SeqRenderingScene::progressGoal() const {
    return m_renderingScene.height();
}
