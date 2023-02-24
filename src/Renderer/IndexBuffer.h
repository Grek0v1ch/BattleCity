#pragma once

#include <glad/glad.h>

namespace RenderEngine {
    class IndexBuffer {
    public:
        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        IndexBuffer() noexcept;
        ~IndexBuffer() noexcept;
        IndexBuffer(IndexBuffer&& o) noexcept;
        IndexBuffer& operator=(IndexBuffer&& o) noexcept;

        void init(const void* data, unsigned int count) noexcept;
        void bind() const noexcept;
        void unbind() const noexcept;
        GLuint getCount() const noexcept { return m_count; }

    private:
        GLuint m_id;
        GLuint m_count;
    };
}