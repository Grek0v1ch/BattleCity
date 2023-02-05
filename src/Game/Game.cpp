#include "Game.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "../Renderer/ShaderProgram.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

Game::Game(const glm::vec2& windowSize) noexcept :
           m_eCurrentGameState(EGameState::Active) ,
           m_windowSize(windowSize) {
    m_keys.fill(false);
}

void Game::render() {
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
}

void Game::update(uint64_t delta) {
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
}

void Game::setKey(const int key, const int action) noexcept {
    m_keys[key] = action;
}

void Game::init() {
    auto pSpriteShaderProgram = ResourceManager::loadShaders("SpriteShader",
                                                             "res/shaders/vSprite.txt",
                                                             "res/shaders/fSprite.txt");
    auto tex = ResourceManager::loadTexture("DefaultTexture",
                                            "res/textures/map_16x16.png");

    std::vector<std::string> subTexturesNames {
            "block",
            "topBlock",
            "bottomBlock",
            "leftBlock",
            "rightBlock",
            "topLeftBlock",
            "topRightBlock",
            "bottomLeftBlock",

            "bottomRightBlock",
            "beton",
            "topBeton",
            "bottomBeton",
            "leftBeton",
            "rightBeton",
            "topLeftBeton",
            "topRightBeton",

            "bottomLeftBeton",
            "bottomRightBeton",
            "water1",
            "water2",
            "water3",
            "trees",
            "ice",
            "wall",

            "eagle",
            "deadEagle",
            "nothing",
            "respawn1",
            "respawn2",
            "respawn3",
            "respawn4"
    };

    auto pTextureAtlas = ResourceManager::loadTextureAtlas("DefaultTextureAtlas",
                                                           "res/textures/map_16x16.png",
                                                           subTexturesNames,
                                                           16, 16);

    auto pSprite = ResourceManager::loadSprite("NewSprite",
                                               "DefaultTextureAtlas",
                                               "SpriteShader",
                                               100, 100,
                                               "beton");
    pSprite->setPosition(glm::vec2(300, 100));

    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite",
                                                               "DefaultTextureAtlas",
                                                               "SpriteShader",
                                                               100, 100,
                                                               "beton");
    pAnimatedSprite->setPosition(glm::vec2(300, 300));
    VectorState waterState;
    waterState.emplace_back(std::make_pair("water1", 1000000000));
    waterState.emplace_back(std::make_pair("water2", 1000000000));
    waterState.emplace_back(std::make_pair("water3", 1000000000));

    VectorState eagleState;
    eagleState.emplace_back(std::make_pair("eagle", 1000000000));
    eagleState.emplace_back(std::make_pair("deadEagle", 1000000000));

    pAnimatedSprite->insertState("waterState", std::move(waterState));
    pAnimatedSprite->insertState("eagleState", std::move(eagleState));


    pAnimatedSprite->setState("waterState");

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x),
                                            0.f, static_cast<float>(m_windowSize.y),
                                            -100.f, 100.f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setUniform("tex", 0);
    pSpriteShaderProgram->setUniform("projectionMat", projectionMatrix);

    pAnimatedSprite->setState("waterState");
}