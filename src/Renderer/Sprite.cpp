#include "Sprite.h"

#include "ShaderProgram.h"
#include "Renderer.h"
#include "Texture2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine {

    Sprite::Sprite(std::shared_ptr<Texture2D> pTexture,
                   const std::string& initialSubTexture,
                   std::shared_ptr<ShaderProgram> pShaderProgram,
                   const glm::vec2& position,
                   const glm::vec2& size,
                   const float rotation) :

                   m_pTexture(std::move(pTexture)),
                   m_pShaderProgram(std::move(pShaderProgram)),
                   m_position(position),
                   m_size(size),
                   m_rotation(rotation) {
        const GLfloat vertexCoords[] {
            // 1---2
            // | / |
            // 0  -3

            // X  Y
            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,
            1.f, 0.f
        };

        auto subTexture = m_pTexture->getSubTexture(initialSubTexture);

        const GLfloat textureCoords[] {
            // U  V
            subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
            subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
            subTexture.rightTopUV.x,   subTexture.rightTopUV.y,
            subTexture.rightTopUV.x,   subTexture.leftBottomUV.y,
        };

        const GLuint indices[] {
            0, 1, 2,
            2, 3, 0
        };

        m_vertexCoordsBuffer.init(vertexCoords, 2 * 4 * sizeof(GLfloat));
        VertexBufferLayout vertexCoordsLayout;
        vertexCoordsLayout.addElementLayout(2, false);
        m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

        m_textureCoordsBuffer.init(textureCoords, 2 * 4 * sizeof(GLfloat));
        VertexBufferLayout textureCoordsLayout;
        textureCoordsLayout.addElementLayout(2, false);
        m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

        m_indexBuffer.init(indices, 6 * sizeof(GLuint));

        m_vertexArray.unbind();
        m_indexBuffer.unbind();
    }

    void Sprite::render() const {
        m_pShaderProgram->use();

        glm::mat4 model(1.f);

        model = glm::translate(model, glm::vec3(m_position, 0.f));
        model = glm::translate(model, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.f));
        model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.f, 0.f, 1.f));
        model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.f));
        model = glm::scale(model, glm::vec3(m_size, 1.f));

        m_pShaderProgram->setUniform("modelMat", model);

        glActiveTexture(GL_TEXTURE0);
        m_pTexture->bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        Renderer::draw(m_vertexArray, m_indexBuffer, *m_pShaderProgram);
    }

    void Sprite::setPosition(const glm::vec2& position) {
        m_position = position;
    }

    void Sprite::setSize(const glm::vec2& size) {
        m_size = size;
    }

    void Sprite::setRotation(const float rotation) {
        m_rotation = rotation;
    }
}