#pragma once

#include <memory>

#include <glm/vec2.hpp>

namespace RenderEngine {
    class AnimatedSprite;
}

class Tank {
public:
    enum class EOrientation {
        Top,
        Bottom,
        Left,
        Right
    };

    Tank(std::shared_ptr<RenderEngine::AnimatedSprite> pSprite, float velocity, const glm::vec2& position);

    void render() const;
    void setOrientation(const EOrientation eOrientation);
    void move(bool move);
    void update(uint64_t delta);

private:
    EOrientation m_eOrientation;
    std::shared_ptr<RenderEngine::AnimatedSprite> m_pSprite;
    bool m_move;
    float m_velocity;
    glm::vec2 m_position;
    glm::vec2 m_moveOffset;
};
