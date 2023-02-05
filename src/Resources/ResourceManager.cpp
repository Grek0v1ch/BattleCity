#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"
#include "../Exception/Exception.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ShaderProgramMap ResourceManager::m_shaderPrograms;
ResourceManager::TextureMap ResourceManager::m_textures;
ResourceManager::SpriteMap ResourceManager::m_sprites;
ResourceManager::AnimatedSpriteMap ResourceManager::m_animatedSprite;
// Путь к ресурсам
std::string ResourceManager::m_resourcePath;

 void ResourceManager::setExecutablePath(const std::string& executablePath) noexcept {
    std::size_t found = executablePath.find_last_of("/\\");
    m_resourcePath = executablePath.substr(0, found);
}

void ResourceManager::unloadAllResources() {
     m_shaderPrograms.clear();
     m_textures.clear();
     m_sprites.clear();
     m_animatedSprite.clear();
     m_resourcePath.clear();
 }

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName,
                                                                      const std::string& vertexPath,
                                                                      const std::string& fragmentPath){
    std::string vertexString = getFileString(vertexPath);
    if (vertexString.empty()) {
        throw Exception::Exception("No vertex shader!");
    }
    std::string fragmentString = getFileString(fragmentPath);
    if (fragmentString.empty()) {
        throw Exception::Exception("No fragment shader!");
    }
    try {
        auto temp = m_shaderPrograms.emplace(shaderName,
                                             std::make_shared<Renderer::ShaderProgram>(vertexString,
                                                                                       fragmentString));
        return temp.first->second;
    } catch (Exception::Exception& ex) {
        std::string msg = "\nCan't load shader program:\nVertex: ";
        msg += vertexPath + "\nFragment: ";
        msg += fragmentPath + "\n";
        ex.addMsg(msg);
        throw ex;
    }
}

std::shared_ptr<Renderer::ShaderProgram>
ResourceManager::getShaderProgram(const std::string& shaderName) noexcept {
    auto it = m_shaderPrograms.find(shaderName);
    if (it != m_shaderPrograms.end()) {
        return it->second;
    }
    std::cerr << "Can't find the shader program: " << shaderName << std::endl;
    return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTexture(const std::string& textureName,
                                                                  const std::string& texturePath) {
    int channels = 0;
    int width = 0;
    int height = 0;

    // Чтобы картинки читались снизу вверх, а не сверху вних как обычно
    stbi_set_flip_vertically_on_load(true);

    unsigned char* pixels = stbi_load(std::string(m_resourcePath + "/" + texturePath).c_str(),
                                      &width, &height, &channels, 0);
    if (! pixels) {
        std::cerr << "Can't load image: " << texturePath << std::endl;
        return nullptr;
    }

    auto temp = m_textures.emplace(textureName,
                                   std::make_shared<Renderer::Texture2D>(width, height,
                                                                         pixels,
                                                                         channels,
                                                                         GL_NEAREST,
                                                                         GL_CLAMP_TO_EDGE));
    auto newTexture = temp.first->second;
    stbi_image_free(pixels);
    return newTexture;
}

std::shared_ptr<Renderer::Texture2D>
ResourceManager::getTexture(const std::string& textureName) noexcept {
    auto it = m_textures.find(textureName);
    if (it != m_textures.end()) {
        return it->second;
    }
    std::cerr << "Can't find the texture: " << textureName << std::endl;
    return nullptr;
}

std::shared_ptr<Renderer::Sprite>
ResourceManager::loadSprite(const std::string& spriteName,
                            const std::string& textureName, const std::string& shaderName,
                            const unsigned int spriteWidth, const unsigned int spriteHeight,
                            const std::string& subTextureName) {
    auto pTexture = getTexture(textureName);
    if (! pTexture) {
        throw Exception::Exception("Can't find the texture: " + textureName +
                                   " for the sprite: " + spriteName);
    }

    auto pShaderProgram = getShaderProgram(shaderName);
    if (! pShaderProgram) {
        throw Exception::Exception("Can't find the shader program: " + shaderName +
                                   " for the sprite: " + spriteName);
    }

    auto tmp = m_sprites.emplace(spriteName,
                                 std::make_shared<Renderer::Sprite>(pTexture,
                                                                    subTextureName,
                                                                    pShaderProgram,
                                                                    glm::vec2(0, 0),
                                                                    glm::vec2(spriteWidth, spriteHeight),
                                                                    0));
    auto newSprite = tmp.first->second;
    return newSprite;
}

std::shared_ptr<Renderer::Sprite>
ResourceManager::getSprite(const std::string& spriteName) noexcept {
    auto it = m_sprites.find(spriteName);
    if (it != m_sprites.end()) {
        return it->second;
    }
    std::cerr << "Can't find the sprite: " << spriteName << std::endl;
    return nullptr;
}

std::shared_ptr<Renderer::Texture2D>
ResourceManager::loadTextureAtlas(const std::string& textureName,
                                  const std::string& texturePath,
                                  const std::vector<std::string>& subTextures,
                                  const unsigned int subTextureWidth,
                                  const unsigned int subTextureHeight) {
    auto pTexture = loadTexture(textureName, texturePath);
    const unsigned int textureWidth = pTexture->width();
    const unsigned int textureHeight = pTexture->height();
    unsigned int currentTextureOffsetX = 0;
    unsigned int currentTextureOffsetY = textureHeight;
    for (const auto& currentSubTextureName : subTextures) {
        glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth,
                               static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);

        glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth,
                             static_cast<float>(currentTextureOffsetY) / textureHeight);

        pTexture->addSubTexture(currentSubTextureName, leftBottomUV, rightTopUV);

        currentTextureOffsetX += subTextureWidth;
        if (currentTextureOffsetX >= textureWidth) {
            currentTextureOffsetX = 0;
            currentTextureOffsetY -= subTextureHeight;
        }
    }

    return pTexture;
}

std::shared_ptr<Renderer::AnimatedSprite>
ResourceManager::loadAnimatedSprite(const std::string& spriteName,
                                    const std::string& textureName,
                                    const std::string& shaderName,
                                    unsigned int spriteWidth, unsigned int spriteHeight,
                                    const std::string& subTextureName) {
    auto pTexture = getTexture(textureName);
    if (! pTexture) {
        throw Exception::Exception("Can't find the texture: " + textureName +
                                   " for the sprite: " + spriteName);
    }

    auto pShaderProgram = getShaderProgram(shaderName);
    if (! pShaderProgram) {
        throw Exception::Exception("Can't find the shader program: " + shaderName +
                                   " for the sprite: " + spriteName);
    }

    auto tmp = m_animatedSprite.emplace(spriteName,
                                        std::make_shared<Renderer::AnimatedSprite>(pTexture,
                                                                                   subTextureName,
                                                                                   pShaderProgram,
                                                                                   glm::vec2(0, 0),
                                                                                   glm::vec2(spriteWidth, spriteHeight),
                                                                                   0));
    auto newSprite = tmp.first->second;
    return newSprite;
}

std::shared_ptr<Renderer::AnimatedSprite>
ResourceManager::getAnimatedSprite(const std::string& spriteName) noexcept {
    auto it = m_animatedSprite.find(spriteName);
    if (it != m_animatedSprite.end()) {
        return it->second;
    }
    std::cerr << "Can't find animated sprite: " << spriteName << std::endl;
    return nullptr;
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath) noexcept {
    std::ifstream fin(m_resourcePath + "/" + relativeFilePath, std::ios::binary);
    if (! fin.is_open()) {
        std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
        return {};
    }
    std::stringstream buffer;
    buffer << fin.rdbuf();
    return buffer.str();
}