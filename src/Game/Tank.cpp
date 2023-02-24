#include "Tank.h"
#include <string>
#include "../Renderer/AnimatedSprite.h"

Tank::Tank(std::shared_ptr<RenderEngine::AnimatedSprite> pSprite, float velocity, const glm::vec2& position)
    :
    m_eOrientation(EOrientation::Top),
    m_pSprite(std::move(pSprite)),
    m_move(false),
    m_velocity(velocity),
    m_position(position),
    m_moveOffset(glm::vec2(0, 1)) {
    m_pSprite->setPosition(m_position);
}

void Tank::render() const {
    m_pSprite->render();
}

void Tank::update(uint64_t delta) {
    if (m_move) {
        m_position += static_cast<float>(delta) * m_velocity * m_moveOffset;
        m_pSprite->setPosition(m_position);
        m_pSprite->update(delta);
    }
}

void Tank::setOrientation(const Tank::EOrientation eOrientation) {
    if (m_eOrientation == eOrientation) {
        return;
    }
    m_eOrientation = eOrientation;
    switch (m_eOrientation) {
        case EOrientation::Top:
            m_pSprite->setState("tankTopState");
            m_moveOffset.x = 0.0f;
            m_moveOffset.y = 1.0f;
            break;
        case EOrientation::Bottom:
            m_pSprite->setState("tankBottomState");
            m_moveOffset.x = 0.0f;
            m_moveOffset.y = -1.0f;
            break;
        case EOrientation::Left:
            m_pSprite->setState("tankLeftState");
            m_moveOffset.x = -1.0f;
            m_moveOffset.y = 0.0f;
            break;
        case EOrientation::Right:
            m_pSprite->setState("tankRightState");
            m_moveOffset.x = 1.0f;
            m_moveOffset.y = 0.0f;
            break;
        default:
            break;
    }
}

void Tank::move(bool move) {
    m_move = move;
}