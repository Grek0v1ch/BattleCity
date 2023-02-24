#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <glad/glad.h>

namespace RenderEngine {
    class VertexArray {
    public:
        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        VertexArray() noexcept;
        ~VertexArray() noexcept;
        VertexArray(VertexArray&& o) noexcept;
        VertexArray& operator=(VertexArray&& o) noexcept;

        void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
        void bind() const noexcept;
        void unbind() const noexcept;

    private:
        GLuint m_id = 0;
        size_t m_elementsCount = 0;
    };
}
