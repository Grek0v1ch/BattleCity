#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>


namespace Renderer {
    /**
     * Класс шейдерной программы для OpenGL. Программа содержит в себе вершинный и фрагментный
     * шейдеры.
     * */
    class ShaderProgram {
    public:
        // Не имеет смысла создавать шейдерную программу без исходного кода шейдеров.
        ShaderProgram() = delete;
        // Нельзя копировать шейдерную программу, иначе у двух шейдеров будут одинаковые
        // идентификаторы, что при работе деструктора вызовет проблемы.
        ShaderProgram(ShaderProgram&) = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

    public:
        /**
         * Конструктор шейдерной программы.
         * @param vertexShader исходный код вершинного шейдера.
         * @param fragmentShader исходный код фрагментного шейдера.
         * @throw Exception::Exception сообщение будет содержать причину ошибки и указание на
         * шейдер, в котором была найдена ошибка.
         * */
        ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
        /**
         * Конструктор перемещения шейдерной программы. Полям shaderProgram будут присвоены
         * значения по умолчанию.
         * @param shaderProgram шейдерная программа для перемещения
         * */
        ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
        ~ShaderProgram() noexcept;

    public:
        ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;

    public:
        bool isCompiled() const { return m_isCompiled; }
        /**
         * Метод запускает шейдерную программу.
         * */
        void use() const noexcept;
        /**
         * Метод устанавливает значение соответствующего uniform.
         * @param name имя uniform.
         * @param value устанавливаемое значение.
         * @throw Exception::Exception в случае, если uniform не был найден.
         * */
        void setUniform(const std::string& name, GLint value);
        /**
         * Метод устанавливает значение соответствующего uniform.
         * @param name имя uniform.
         * @param value устанавливаемое значение.
         * @throw Exception::Exception в случае, если uniform не был найден.
         * */
        void setUniform(const std::string& name, glm::mat4 matrix);

    private:
        // Поле показывает, собралась шейдерная программа или нет.
        bool m_isCompiled = false;
        // Идентификатор шейдерной программы.
        GLuint m_ID = 0;

    private:
        /**
         * Метод создает шейдер.
         * @param source исходный код создаваемого шейдера
         * @param shaderType тип создаваемого шейдера
         * @param shaderID в данный параметр будет записан идентификатор созданного шейдера
         * @throw Exception::Exception сообщение с ошибкой компиляции шейдера.
         * */
        void createShader(const std::string& source, GLenum shaderType, GLuint& shaderID);
    };
}