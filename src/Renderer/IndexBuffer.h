#pragma once

#include <glad/glad.h>

namespace Renderer {
    class IndexBuffer {
    public:
        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        IndexBuffer() noexcept;
        ~IndexBuffer() noexcept;
        IndexBuffer(IndexBuffer&& o) noexcept;
        IndexBuffer& operator=(IndexBuffer&& o) noexcept;

        void init(const void* data, unsigned int size) noexcept;
        void bind() const noexcept;
        void unbind() const noexcept;

    private:
        GLuint m_id;
    };
}