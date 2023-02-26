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

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

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

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName,
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
                                             std::make_shared<RenderEngine::ShaderProgram>(vertexString,
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

std::shared_ptr<RenderEngine::ShaderProgram>
ResourceManager::getShaderProgram(const std::string& shaderName) noexcept {
    auto it = m_shaderPrograms.find(shaderName);
    if (it != m_shaderPrograms.end()) {
        return it->second;
    }
    std::cerr << "Can't find the shader program: " << shaderName << std::endl;
    return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTexture(const std::string& textureName,
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
                                   std::make_shared<RenderEngine::Texture2D>(width, height,
                                                                             pixels,
                                                                             channels,
                                                                             GL_NEAREST,
                                                                             GL_CLAMP_TO_EDGE));
    auto newTexture = temp.first->second;
    stbi_image_free(pixels);
    return newTexture;
}

std::shared_ptr<RenderEngine::Texture2D>
ResourceManager::getTexture(const std::string& textureName) noexcept {
    auto it = m_textures.find(textureName);
    if (it != m_textures.end()) {
        return it->second;
    }
    std::cerr << "Can't find the texture: " << textureName << std::endl;
    return nullptr;
}

std::shared_ptr<RenderEngine::Sprite>
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
                                 std::make_shared<RenderEngine::Sprite>(pTexture,
                                                                        subTextureName,
                                                                        pShaderProgram,
                                                                        glm::vec2(0, 0),
                                                                        glm::vec2(spriteWidth, spriteHeight),
                                                                        0));
    auto newSprite = tmp.first->second;
    return newSprite;
}

std::shared_ptr<RenderEngine::Sprite>
ResourceManager::getSprite(const std::string& spriteName) noexcept {
    auto it = m_sprites.find(spriteName);
    if (it != m_sprites.end()) {
        return it->second;
    }
    std::cerr << "Can't find the sprite: " << spriteName << std::endl;
    return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D>
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

std::shared_ptr<RenderEngine::AnimatedSprite>
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
                                        std::make_shared<RenderEngine::AnimatedSprite>(pTexture,
                                                                                       subTextureName,
                                                                                       pShaderProgram,
                                                                                       glm::vec2(0, 0),
                                                                                       glm::vec2(spriteWidth, spriteHeight),
                                                                                       0));
    auto newSprite = tmp.first->second;
    return newSprite;
}

std::shared_ptr<RenderEngine::AnimatedSprite>
ResourceManager::getAnimatedSprite(const std::string& spriteName) noexcept {
    auto it = m_animatedSprite.find(spriteName);
    if (it != m_animatedSprite.end()) {
        return it->second;
    }
    std::cerr << "Can't find animated sprite: " << spriteName << std::endl;
    return nullptr;
}

bool ResourceManager::loadJSONResources(const std::string& JSONPath) noexcept {
     const std::string JSONString = getFileString(JSONPath);
     if (JSONString.empty()) {
         std::cerr << "No JSON resources file" << std::endl;
         return false;
     }
     rapidjson::Document document;
     rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());
     if (! parseResult) {
         std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code())
                   << " (" << parseResult.Offset() << ")" << std::endl;
         std::cerr << "In JSON file: " << JSONPath << std::endl;
         return false;
     }

     auto shadersIt = document.FindMember("shaders");
     if (shadersIt != document.MemberEnd()) {
         for (const auto& currShader : shadersIt->value.GetArray()) {
             const std::string name = currShader["name"].GetString();
             const std::string filePath_v = currShader["filePath_v"].GetString();
             const std::string filePath_f = currShader["filePath_f"].GetString();
             loadShaders(name, filePath_v, filePath_f);
         }
     }

     auto textureAtlasesIt = document.FindMember("textureAtlases");
     if (textureAtlasesIt != document.MemberEnd()) {
         for (const auto& currTextureAtlases : textureAtlasesIt->value.GetArray()) {
             const std::string name = currTextureAtlases["name"].GetString();
             const std::string filePath = currTextureAtlases["filePath"].GetString();
             const unsigned int subTextureWidth = currTextureAtlases["subTextureWidth"].GetUint();
             const unsigned int subTextureHeight = currTextureAtlases["subTextureHeight"].GetUint();

             const auto subTexturesArray = currTextureAtlases["subTextures"].GetArray();
             std::vector<std::string> subTextures;
             subTextures.reserve(subTexturesArray.Size());
             for (const auto& currSubTexture : subTexturesArray) {
                 subTextures.emplace_back(currSubTexture.GetString());
             }
             loadTextureAtlas(name, filePath, subTextures, subTextureWidth, subTextureHeight);
         }
     }

     auto animatedSpriteIt = document.FindMember("animatedSprites");
     if (animatedSpriteIt != document.MemberEnd()) {
         for (const auto& currAnimatedSprite : animatedSpriteIt->value.GetArray()) {
             const std::string name = currAnimatedSprite["name"].GetString();
             const std::string textureAtlas = currAnimatedSprite["textureAtlas"].GetString();
             const std::string shader = currAnimatedSprite["shader"].GetString();
             const unsigned int initialWidth = currAnimatedSprite["initialWidth"].GetUint();
             const unsigned int initialHeight = currAnimatedSprite["initialHeight"].GetUint();
             const std::string initialSubTexture = currAnimatedSprite["initialSubTexture"].GetString();

            auto pAnimatedSprite = loadAnimatedSprite(name, textureAtlas, shader,
                                                      initialWidth, initialHeight,
                                                      initialSubTexture);
            if (! pAnimatedSprite) {
                continue;
            }

            const auto statesArray = currAnimatedSprite["states"].GetArray();
            for (const auto& currState : statesArray) {
                const std::string stateName = currState["stateName"].GetString();
                std::vector<std::pair<std::string, uint64_t>> frames;
                const auto framesArray = currState["frames"].GetArray();
                frames.reserve(framesArray.Size());
                for (const auto& currFrame : framesArray) {
                    const std::string subTexture = currFrame["subTexture"].GetString();
                    const uint64_t duration = currFrame["duration"].GetUint64();
                    frames.emplace_back(subTexture, duration);
                }
                pAnimatedSprite->insertState(stateName, std::move(frames));
            }
         }
     }
     return true;
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