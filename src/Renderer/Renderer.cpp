#include "Renderer.h"

namespace RenderEngine {
    void Renderer::draw(const RenderEngine::VertexArray& vertexArray,
                        const RenderEngine::IndexBuffer& indexBuffer,
                        const RenderEngine::ShaderProgram& shaderProgram) noexcept {
        shaderProgram.use();
        vertexArray.bind();
        indexBuffer.bind();

        glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::setClearColour(const GLfloat r, const GLfloat g, const GLfloat b,
                                  const GLfloat a) noexcept {
        glClearColor(r, g, b, a);
    }

    void Renderer::clear() noexcept {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::setViewport(GLuint width, GLuint height, GLuint leftOffset,
                               GLuint bottomOffset) noexcept {
        glViewport(leftOffset, bottomOffset, width, height);
    }

    std::string Renderer::getRendererStr() noexcept {
        return { reinterpret_cast<const char*>(glGetString(GL_RENDERER)) };
    }

    std::string Renderer::getVersionStr() noexcept {
        return { reinterpret_cast<const char*>(glGetString(GL_VERSION)) };
    }
}
