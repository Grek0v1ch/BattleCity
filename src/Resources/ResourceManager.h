#pragma once

#include <string>
#include <map>
#include <memory>

namespace Renderer {
    class ShaderProgram;
    class Texture2D;
}

class ResourceManager {
public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

public:
    ResourceManager(const std::string& executablePath);
    ~ResourceManager() = default;

public:
    std::shared_ptr<Renderer::ShaderProgram> loadShaders(const std::string& shaderName,
                                                         const std::string& vertexPath,
                                                         const std::string& fragmentPath);
    std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderName);

    std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName,
                                                     const std::string& texturePath);
    std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName);


private:
    std::string getFileString(const std::string& relativeFilePath) const;

private:
    using ShaderProgramMap = std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>>;
    using TextureMap = std::map<std::string, std::shared_ptr<Renderer::Texture2D>>;

    ShaderProgramMap m_shaderPrograms;
    TextureMap m_textures;

    // Путь к ресурсам
    std::string m_resourcePath;
};