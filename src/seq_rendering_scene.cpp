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

size_t SeqRenderingScene::progressWork() {
    return m_row;
}

size_t SeqRenderingScene::progressGoal() {
    return m_renderingScene.height();
}
