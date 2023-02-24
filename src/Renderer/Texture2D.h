#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <string>
#include <map>

namespace RenderEngine {
    /**
    * Класс двумерной текстуры.
    * */
    class Texture2D {
    public:
        Texture2D() = delete;
        Texture2D(const Texture2D&) = delete;
        Texture2D(Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;

    public:
        struct SubTexture2D {
            glm::vec2 leftBottomUV;
            glm::vec2 rightTopUV;

            SubTexture2D(const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV) :
                leftBottomUV(_leftBottomUV), rightTopUV(_rightTopUV) {}

            SubTexture2D() : leftBottomUV(0.0f), rightTopUV(1.0f) {}
        };
        /**
         * @param height высота
         * @param width ширина
         * @param data массив с данными текстуры
         * @param channels канальность цвета (по умолчанию 4)
         * @param filter текстурный фильтр (по умолчанию GL_LINEAR)
         * @param wrapMode опция wrapping (по умолчанию GL_CLAMP_TO_EDGE)
         * */
        Texture2D(GLint width, GLint height,
                  const unsigned char* data,
                  unsigned int channels = 4,
                  GLint filter = GL_LINEAR,
                  GLint wrapMode = GL_CLAMP_TO_EDGE) noexcept;
        Texture2D(Texture2D&& texture2D) noexcept;
        ~Texture2D() noexcept;

    public:
        Texture2D& operator=(Texture2D&& texture2D) noexcept;

        /**
         * Метод подключает текстуру к цели GL_TEXTURE_2D.
         * */
        void bind() const noexcept;

        void addSubTexture(const std::string& name,
                           const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);
        const SubTexture2D getSubTexture(const std::string& name) const;
        unsigned int width() const noexcept { return m_width; }
        unsigned int height() const noexcept {return m_height; }

    private:
        GLint m_width;
        GLint m_height;
        GLuint m_ID;
        GLint m_mode;

        std::map<std::string, SubTexture2D> m_subTextures;
    };
}