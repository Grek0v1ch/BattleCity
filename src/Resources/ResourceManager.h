#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace Renderer {
    class ShaderProgram;
    class Texture2D;
    class Sprite;
}

class ResourceManager {
public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

public:
    /**
     * Конструктор класса.
     * @param executablePath абсолютный путь к исполняемому файлу. Папка, в которой лежит исполняемый
     * файл будет считать основной, и все относительный пути будут вестись от нее.
     * */
    ResourceManager(const std::string& executablePath) noexcept;
    ~ResourceManager() = default;

public:
    std::shared_ptr<Renderer::ShaderProgram> loadShaders(const std::string& shaderName,
                                                         const std::string& vertexPath,
                                                         const std::string& fragmentPath);
    /**
     * Метод возвращает указатель на шейдерную программу с заданным именем.
     * @param shaderName имя шейдерной программы для поиска.
     * @return указатель на шейдерную программу или nullptr, если она не была найдена.\n
     * В случае, если шейдерная программа не была найдена, в std::cerr будет выведено
     * соответствующее сообщение.
     * */
    std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderName) noexcept;

    std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName,
                                                     const std::string& texturePath);
    /**
     * Метод возвращает указатель на текстуру с заданным именем.
     * @param textureName имя текстуры для поиска.
     * @return указатель на текстуру или nullptr, если она не была найдена.\n
     * В случае, если текстура не была найдена, в std::cerr будет выведено
     * соответствующее сообщение.
     * */
    std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName) noexcept;

    std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName,
                                                 const std::string& textureName,
                                                 const std::string& shaderName,
                                                 unsigned int spriteWidth, unsigned int spriteHeight,
                                                 const std::string& subTextureName = "default");

    std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName) noexcept;

    std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(const std::string& textureName,
                                                          const std::string& texturePath,
                                                          const std::vector<std::string>& subTextures,
                                                          unsigned int subTextureWidth,
                                                          unsigned int subTextureHeight);
private:
    /**
     * Метод читает в std::string весь переданный файл.
     * @param relativeFilePath путь к файлу, относительно папки с исполняемым файлом.
     * @return в случае, если прочитать файл не удалось, будет возвращена пустая строка. В std::cerr
     * будет выведено сообщение.
     * */
    std::string getFileString(const std::string& relativeFilePath) const noexcept;

private:
    using ShaderProgramMap = std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>>;
    using TextureMap = std::map<std::string, std::shared_ptr<Renderer::Texture2D>>;
    using SpriteMap = std::map<std::string, std::shared_ptr<Renderer::Sprite>>;

    ShaderProgramMap m_shaderPrograms;
    TextureMap m_textures;
    SpriteMap m_sprites;

    // Путь к ресурсам
    std::string m_resourcePath;
};