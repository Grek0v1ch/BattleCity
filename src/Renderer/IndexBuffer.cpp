#include "IndexBuffer.h"

namespace RenderEngine {
    IndexBuffer::IndexBuffer() noexcept : m_id(0), m_count(0) {}

    IndexBuffer::~IndexBuffer() noexcept {
        glDeleteBuffers(1, &m_id);
    }

    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& o) noexcept {
        m_id = o.m_id;
        o.m_id = 0;
        m_count = o.m_count;
        o.m_count = 0;
        return *this;
    }

    IndexBuffer::IndexBuffer(RenderEngine::IndexBuffer&& o) noexcept {
        m_id = o.m_id;
        o.m_id = 0;
        m_count = o.m_count;
        o.m_count = 0;
    }

    void IndexBuffer::init(const void *data, const unsigned int count) noexcept {
        m_count = count;
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), data, GL_STATIC_DRAW);
    }

    void IndexBuffer::bind() const noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::unbind() const noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}