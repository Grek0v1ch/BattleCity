#pragma once

#include <glad/glad.h>
#include <string>

namespace Renderer {
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

    private:
        GLint m_width;
        GLint m_height;
        GLuint m_ID;
        GLint m_mode;
    };
}