#include <utility>

#include "seq_rendering_scene.hpp"

SeqRenderingScene::SeqRenderingScene(RenderingScene &&renderingScene):
    m_renderingScene(std::move(renderingScene)) {
}

size_t SeqRenderingScene::progressWork() {
    if (m_row >= m_renderingScene.height()) {
        return m_renderingScene.height();
    }

    m_renderingScene.renderRow(m_row++);

    return m_row;
}

size_t SeqRenderingScene::progressGoal() {
    return m_renderingScene.height();
}
