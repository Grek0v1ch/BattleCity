#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#include "../Exception/Exception.h"

namespace RenderEngine {
    ShaderProgram::ShaderProgram(const std::string& vertexShader,
                                 const std::string& fragmentShader) {
        GLuint vertexShaderID;
        try {
            createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID);
        } catch (Exception::Exception& ex) {
            ex.addMsg("VERTEX SHADER compile-time error\n");
            throw ex;
        }

        GLuint fragmentShaderID;
        try {
            createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID);
        } catch (Exception::Exception& ex) {
            glDeleteShader(vertexShaderID);
            ex.addMsg("FRAGMENT SHADER compile-time error\n");
            throw ex;
        }

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShaderID);
        glAttachShader(m_ID, fragmentShaderID);
        glLinkProgram(m_ID);
        // Удаляем шейдеры, так как они больше не нужны, потому что уже привязаны к шейдерной
        // программе.
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        GLint success = 0;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (! success) {
            GLchar infoLog[1024];
            glGetProgramInfoLog(m_ID, 1024, nullptr, infoLog);
            std::string msg("ERROR::SHADER: Linking-time error\n");
            msg += infoLog;
            throw Exception::Exception(msg);
        } else {
            m_isCompiled = true;
        }
    }

    ShaderProgram::ShaderProgram(RenderEngine::ShaderProgram&& shaderProgram) noexcept {
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;
    }

    ShaderProgram::~ShaderProgram() noexcept {
        glDeleteProgram(m_ID);
    }

    void ShaderProgram::use() const noexcept {
        glUseProgram(m_ID);

    }

    void ShaderProgram::setUniform(const std::string& name, const GLint value) {
        auto location = glGetUniformLocation(m_ID, name.c_str());
        if (location == -1) {
            throw Exception::Exception(
                    "ERROR::SHADER_PROGRAM: there is no uniform with the name" +
                    name);
        }
        glUniform1i(location, value);
    }

    void ShaderProgram::setUniform(const std::string& name, const glm::mat4 matrix) {
        auto location = glGetUniformLocation(m_ID, name.c_str());
        if (location == -1) {
            throw Exception::Exception(
                    "ERROR::SHADER_PROGRAM: there is no uniform with the name " +
                    name);
        }
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    ShaderProgram& ShaderProgram::operator=(RenderEngine::ShaderProgram&& shaderProgram) noexcept {
        glDeleteProgram(m_ID);
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;
        return *this;
    }

    void ShaderProgram::createShader(const std::string& source,
                                     const GLenum shaderType,
                                     GLuint& shaderID) {
        shaderID = glCreateShader(shaderType);
        const GLchar* code = source.c_str();
        glShaderSource(shaderID, 1, &code, nullptr);
        glCompileShader(shaderID);

        GLint success = 0;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (! success) {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
            throw Exception::Exception(infoLog);
        }
    }
}