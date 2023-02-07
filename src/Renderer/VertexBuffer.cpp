#include "VertexBuffer.h"

namespace Renderer {
    VertexBuffer::VertexBuffer() noexcept : m_id(0) {}

    VertexBuffer::~VertexBuffer() noexcept {
        glDeleteBuffers(1, &m_id);
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& o) noexcept {
        m_id = o.m_id;
        o.m_id = 0;
        return *this;
    }

    VertexBuffer::VertexBuffer(Renderer::VertexBuffer&& o) noexcept {
        m_id = o.m_id;
        o.m_id = 0;
    }

    void VertexBuffer::init(const void *data, const unsigned int size) noexcept {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void VertexBuffer::update(const void *data, const unsigned int size) const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    void VertexBuffer::bind() const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::unbind() const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}