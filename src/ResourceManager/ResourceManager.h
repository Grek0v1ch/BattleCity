#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace RenderEngine {
    class ShaderProgram;
    class Texture2D;
    class Sprite;
    class AnimatedSprite;
}

class ResourceManager {
public:
    ResourceManager() = delete;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ~ResourceManager() = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

public:
    /**
     * Конструктор класса.
     * @param executablePath абсолютный путь к исполняемому файлу. Папка, в которой лежит исполняемый
     * файл будет считать основной, и все относительный пути будут вестись от нее.
     * */
    static void setExecutablePath(const std::string& executablePath) noexcept;
    static void unloadAllResources();

    static std::shared_ptr<RenderEngine::ShaderProgram>
    loadShaders(const std::string& shaderName,
                const std::string& vertexPath, const std::string& fragmentPath);
    /**
     * Метод возвращает указатель на шейдерную программу с заданным именем.
     * @param shaderName имя шейдерной программы для поиска.
     * @return указатель на шейдерную программу или nullptr, если она не была найдена.\n
     * В случае, если шейдерная программа не была найдена, в std::cerr будет выведено
     * соответствующее сообщение.
     * */
    static std::shared_ptr<RenderEngine::ShaderProgram>
    getShaderProgram(const std::string& shaderName) noexcept;

    static std::shared_ptr<RenderEngine::Texture2D>
    loadTexture(const std::string& textureName, const std::string& texturePath);
    /**
     * Метод возвращает указатель на текстуру с заданным именем.
     * @param textureName имя текстуры для поиска.
     * @return указатель на текстуру или nullptr, если она не была найдена.\n
     * В случае, если текстура не была найдена, в std::cerr будет выведено
     * соответствующее сообщение.
     * */
    static std::shared_ptr<RenderEngine::Texture2D> getTexture(const std::string& textureName) noexcept;

    static std::shared_ptr<RenderEngine::Sprite>
    loadSprite(const std::string& spriteName,
               const std::string& textureName, const std::string& shaderName,
               unsigned int spriteWidth, unsigned int spriteHeight,
               const std::string& subTextureName = "default");

    static std::shared_ptr<RenderEngine::Sprite> getSprite(const std::string& spriteName) noexcept;

    static std::shared_ptr<RenderEngine::Texture2D>
    loadTextureAtlas(const std::string& textureName,
                     const std::string& texturePath, const std::vector<std::string>& subTextures,
                     unsigned int subTextureWidth, unsigned int subTextureHeight);

    static std::shared_ptr<RenderEngine::AnimatedSprite>
    loadAnimatedSprite(const std::string& spriteName,
                       const std::string& textureName, const std::string& shaderName,
                       unsigned int spriteWidth, unsigned int spriteHeight,
                       const std::string& subTextureName = "default");

    static std::shared_ptr<RenderEngine::AnimatedSprite>
    getAnimatedSprite(const std::string& spriteName) noexcept;

    static bool loadJSONResources(const std::string& JSONPath) noexcept;
private:
    /**
     * Метод читает в std::string весь переданный файл.
     * @param relativeFilePath путь к файлу, относительно папки с исполняемым файлом.
     * @return в случае, если прочитать файл не удалось, будет возвращена пустая строка. В std::cerr
     * будет выведено сообщение.
     * */
    static std::string getFileString(const std::string& relativeFilePath) noexcept;

private:
    using ShaderProgramMap = std::map<std::string, std::shared_ptr<RenderEngine::ShaderProgram>>;
    using TextureMap = std::map<std::string, std::shared_ptr<RenderEngine::Texture2D>>;
    using SpriteMap = std::map<std::string, std::shared_ptr<RenderEngine::Sprite>>;
    using AnimatedSpriteMap = std::map<std::string, std::shared_ptr<RenderEngine::AnimatedSprite>>;

    static ShaderProgramMap m_shaderPrograms;
    static TextureMap m_textures;
    static SpriteMap m_sprites;
    static AnimatedSpriteMap m_animatedSprite;
    // Путь к ресурсам
    static std::string m_resourcePath;
};