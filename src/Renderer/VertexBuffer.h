#pragma once

#include <glad/glad.h>

namespace RenderEngine {
    class VertexBuffer {
    public:
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        VertexBuffer() noexcept;
        ~VertexBuffer() noexcept;
        VertexBuffer(VertexBuffer&& o) noexcept;
        VertexBuffer& operator=(VertexBuffer&& o) noexcept;

        void init(const void* data, unsigned int size) noexcept;
        void update(const void* data, unsigned int size) const noexcept;
        void bind() const noexcept;
        void unbind() const noexcept;

    private:
        GLuint m_id;
    };
}