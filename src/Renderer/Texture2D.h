#pragma once

#include <glad/glad.h>
#include <string>

namespace Renderer {
    class Texture2D {
    public:
        Texture2D() = delete;
        Texture2D(const Texture2D&) = delete;
        Texture2D(Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;

    public:
        Texture2D(GLuint width, GLuint height,
                  const unsigned char* data,
                  unsigned int channels = 4,
                  GLenum filter = GL_LINEAR,
                  GLenum wrapMode = GL_CLAMP_TO_EDGE);
        Texture2D(Texture2D&& texture2D) noexcept;
        ~Texture2D();

    public:
        Texture2D& operator=(Texture2D&& texture2D) noexcept;

        void bind() const;

    private:
        unsigned int m_width;
        unsigned int m_height;
        GLuint m_ID;
        GLenum m_mode;
    };
}