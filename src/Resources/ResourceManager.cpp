#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Exception/Exception.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ResourceManager(const std::string& executablePath) noexcept {
    std::size_t found = executablePath.find_last_of("/\\");
    m_resourcePath = executablePath.substr(0, found);
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

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(
        const std::string& shaderName) noexcept {
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

std::shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(
        const std::string& textureName) noexcept {
    auto it = m_textures.find(textureName);
    if (it != m_textures.end()) {
        return it->second;
    }
    std::cerr << "Can't find the texture: " << textureName << std::endl;
    return nullptr;
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath) const noexcept {
    std::ifstream fin(m_resourcePath + "/" + relativeFilePath, std::ios::binary);
    if (! fin.is_open()) {
        std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
        return {};
    }
    std::stringstream buffer;
    buffer << fin.rdbuf();
    return buffer.str();
}