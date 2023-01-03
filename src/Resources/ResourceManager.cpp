#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

ResourceManager::ResourceManager(const std::string& executablePath) {
    std::size_t found = executablePath.find_last_of("/\\");
    m_resourcePath = executablePath.substr(0, found);
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName,
                                                                      const std::string& vertexPath,
                                                                      const std::string& fragmentPath){
    std::string vertexString = getFileString(vertexPath);
    if (vertexString.empty()) {
        std::cerr << "No vertex shader!" << std::endl;
        return nullptr;
    }
    std::string fragmentString = getFileString(fragmentPath);
    if (fragmentString.empty()) {
        std::cerr << "No fragment shader!" << std::endl;
        return nullptr;
    }
    auto temp = m_shaderPrograms.emplace(shaderName,
                                         std::make_shared<Renderer::ShaderProgram>(vertexString,
                                                                                   fragmentString));
    auto newShader = temp.first->second;
    if (newShader->isCompiled()) {
        return newShader;
    }
    std::cerr << "Can't load shader program:\n"
        << "Vertex: " << vertexPath << '\n'
        << "Fragment: " << fragmentPath << std::endl;
    return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName) {
    auto it = m_shaderPrograms.find(shaderName);
    if (it != m_shaderPrograms.end()) {
        return it->second;
    }
    std::cerr << "Can't find the shader program: " << shaderName << std::endl;
    return nullptr;
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath) const {
    std::ifstream fin(m_resourcePath + "/" + relativeFilePath, std::ios::binary);
    if (! fin.is_open()) {
        std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
        return {};
    }
    std::stringstream buffer;
    buffer << fin.rdbuf();
    return buffer.str();
}