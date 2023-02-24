#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

namespace RenderEngine {
    class Renderer {
    public:
        static void draw(const VertexArray& vertexArray,
                         const IndexBuffer& indexBuffer,
                         const ShaderProgram& shaderProgram) noexcept;
        static void setClearColour(GLfloat r, GLfloat g, GLfloat b, GLfloat a) noexcept;
        static void clear() noexcept;
        static void setViewport(GLuint width, GLuint height,
                                GLuint leftOffset = 0, GLuint bottomOffset = 0) noexcept;

        static std::string getRendererStr() noexcept;
        static std::string getVersionStr() noexcept;
    };
}
