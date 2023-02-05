#pragma once

#include <array>
#include <glm/vec2.hpp>

class Game {
public:
    Game(const glm::vec2& windowSize) noexcept;
    ~Game() = default;

    void render();
    void update(uint64_t delta);
    void setKey(int key, int action) noexcept;
    void init();

private:
    std::array<bool, 349> m_keys;

    enum class EGameState {
        Active,
        Pause
    };

    EGameState m_eCurrentGameState;
    glm::ivec2 m_windowSize;
};