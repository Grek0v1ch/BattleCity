#pragma once

#include "Sprite.h"

#include <map>
#include <vector>

using VectorState = std::vector<std::pair<std::string, uint64_t>>;

namespace Renderer {

    class AnimatedSprite : public Sprite {
    public:

        /**
         * @param pTexture указатель на текстуру спрайта.
         * @param pShaderProgram шейдерная программа для рендеринга текстуры.
         * @param position позиция спрайта (по умолчанию 0)
         * @param size размер спрайта (по умолчанию 1)
         * @param rotation угол поворота (по умолчанию 0)
         * */
        AnimatedSprite(std::shared_ptr<Texture2D> pTexture,
               const std::string& initialSubTexture,
               std::shared_ptr<ShaderProgram> pShaderProgram,
               const glm::vec2& position = glm::vec2(0.0f),
               const glm::vec2& size = glm::vec2(1.0f),
               float rotation = 0.0f);

        void insertState(std::string state, VectorState subTexturesDuration);
        void render() const override;
        void update(uint64_t delta);
        void setState(const std::string& newState);

    private:
        std::map<std::string, VectorState> m_statesMap;
        size_t m_currentFrame = 0;
        uint64_t m_currentAnimationTime = 0;
        std::map<std::string, VectorState>::const_iterator m_pCurrentAnimationDuration;
        // можно менять в const функциях
        mutable bool m_dirty = false;
    };
}