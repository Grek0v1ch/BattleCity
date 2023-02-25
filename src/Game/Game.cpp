#include "Game.h"

#include <iostream>

#include "../Renderer/ShaderProgram.h"
#include "../ResourceManager/ResourceManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Tank.h"

Game::Game(const glm::vec2& windowSize) noexcept :
           m_eCurrentGameState(EGameState::Active) ,
           m_windowSize(windowSize) {
    m_keys.fill(false);
}

Game::~Game() {}

void Game::render() {
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
    if (m_pTank) {
        m_pTank->render();
    }
}

void Game::update(uint64_t delta) {
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
    if (m_pTank) {
        if (m_keys[GLFW_KEY_W]) {
            m_pTank->setOrientation(Tank::EOrientation::Top);
            m_pTank->move(true);
        } else if (m_keys[GLFW_KEY_A]) {
            m_pTank->setOrientation(Tank::EOrientation::Left);
            m_pTank->move(true);
        } else if (m_keys[GLFW_KEY_S]) {
            m_pTank->setOrientation(Tank::EOrientation::Bottom);
            m_pTank->move(true);
        } else if (m_keys[GLFW_KEY_D]) {
            m_pTank->setOrientation(Tank::EOrientation::Right);
            m_pTank->move(true);
        } else {
            m_pTank->move(false);
        }
        m_pTank->update(delta);
    }
}

void Game::setKey(const int key, const int action) noexcept {
    m_keys[key] = action;
}

void Game::init() {
    ResourceManager::loadJSONResources("res/resources.json");

    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    if (! pSpriteShaderProgram) {
        std::cerr << "Can't find shader program: spriteShader" << std::endl;
        return;
    }

    auto pTextureAtlas = ResourceManager::getTexture("mapTextureAtlas");
    if (! pTextureAtlas) {
        std::cerr << "Can't find texture atlas: mapTextureAtlas" << std::endl;
        return;
    }

    auto pTanksTextureAtlas = ResourceManager::getTexture("tanksTextureAtlas");
    if (! pTanksTextureAtlas) {
        std::cerr << "Can't find texture atlas: tanksTextureAtlas" << std::endl;
        return;
    }

    auto pSprite = ResourceManager::loadSprite("NewSprite",
                                               "mapTextureAtlas",
                                               "spriteShader",
                                               100, 100,
                                               "beton");
    pSprite->setPosition(glm::vec2(300, 100));

    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite",
                                                               "mapTextureAtlas",
                                                               "spriteShader",
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

    auto pTanksAnimatedSprite = ResourceManager::loadAnimatedSprite("TanksAnimatedSprite",
                                                                    "tanksTextureAtlas",
                                                                    "spriteShader",
                                                                    100, 100,
                                                                    "yellowType1_Top1");

    VectorState tankTopState;
    tankTopState.emplace_back(std::make_pair("tankTop1", 500000000));
    tankTopState.emplace_back(std::make_pair("tankTop2", 500000000));

    VectorState tankLeftState;
    tankLeftState.emplace_back(std::make_pair("tankLeft1", 500000000));
    tankLeftState.emplace_back(std::make_pair("tankLeft2", 500000000));

    VectorState tankBottomState;
    tankBottomState.emplace_back(std::make_pair("tankBottom1", 500000000));
    tankBottomState.emplace_back(std::make_pair("tankBottom2", 500000000));

    VectorState tankRightState;
    tankRightState.emplace_back(std::make_pair("tankRight1", 500000000));
    tankRightState.emplace_back(std::make_pair("tankRight2", 500000000));

    pTanksAnimatedSprite->insertState("tankTopState",    tankTopState);
    pTanksAnimatedSprite->insertState("tankLeftState",   tankLeftState);
    pTanksAnimatedSprite->insertState("tankBottomState", tankBottomState);
    pTanksAnimatedSprite->insertState("tankRightState",  tankRightState);

    pTanksAnimatedSprite->setState("tankTopState");

    m_pTank = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001f, glm::vec2(100, 100));
}