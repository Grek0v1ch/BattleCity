#include "AnimatedSprite.h"
#include "../Exception/Exception.h"
#include "Texture2D.h"

#include <string>

namespace Renderer {
    AnimatedSprite::AnimatedSprite(std::shared_ptr<Texture2D> pTexture,
                                   const std::string& initialSubTexture,
                                   std::shared_ptr<ShaderProgram> pShaderProgram,
                                   const glm::vec2& position,
                                   const glm::vec2& size,
                                   float rotation) :
                                   Sprite(std::move(pTexture), initialSubTexture,
                                          std::move(pShaderProgram), position, size, rotation) {
        m_pCurrentAnimationDuration = m_statesMap.end();
    }

    void
    AnimatedSprite::insertState(std::string state, VectorState subTexturesDuration) {
        m_statesMap.emplace(std::move(state), std::move(subTexturesDuration));
    }

    void AnimatedSprite::render() const {
        if (m_dirty) {
            auto subTexture = m_pTexture->getSubTexture(
                    m_pCurrentAnimationDuration->second[m_currentFrame].first);

            const GLfloat textureCoords[]{
                    // u                                  v
                    subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
                    subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
                    subTexture.rightTopUV.x, subTexture.rightTopUV.y,
                    subTexture.rightTopUV.x, subTexture.leftBottomUV.y,
            };

            m_textureCoordsBuffer.update(textureCoords, 2 * 4 * sizeof(GLfloat));
            m_textureCoordsBuffer.unbind();
            m_dirty = false;
        }
        Sprite::render();
    }

    void AnimatedSprite::update(const uint64_t delta) {
        if (m_pCurrentAnimationDuration != m_statesMap.end()) {
            m_currentAnimationTime += delta;
            while (m_currentAnimationTime >= m_pCurrentAnimationDuration->second[m_currentFrame].second) {
                m_currentAnimationTime -= m_pCurrentAnimationDuration->second[m_currentFrame].second;
                ++m_currentFrame;
                m_dirty = true;
                if (m_currentFrame == m_pCurrentAnimationDuration->second.size()) {
                    m_currentFrame = 0;
                }
            }
        }
    }

    void AnimatedSprite::setState(const std::string& newState) {
        auto it = m_statesMap.find(newState);
        if (it == m_statesMap.end()) {
            throw Exception::Exception("Can't find animation state: " + newState);
        }
        if (it != m_pCurrentAnimationDuration) {
            m_currentAnimationTime = 0;
            m_currentFrame = 0;
            m_pCurrentAnimationDuration = it;
            m_dirty = true;
        }
    }
}