#pragma once

#include <glad/glad.h>

#include <string>

namespace Renderer {
    /**
    * Класс шейдерной программы для OpenGL. Содержит в себе вершинный и фрагментный шейдеры.
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
         * Конструктор шейдерной программы. В случае, если по какой-либо причине шейдерную программу
         * создать нельзя, в std::cerr будет выведено сообщение об ошибке, а полям класса будут
         * оставлены значения по умолчанию.
         * @param vertexShader исходный код вершинного шейдера
         * @param fragmentShader исходный код фрагментного шейдера
         * */
        ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
        /**
         * Конструктор перемещения шейдерной программы. Полям shaderProgram будут присвоены
         * значения по умолчанию.
         * @param shaderProgram шейдерная программа для перемещения
         * */
        ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
        ~ShaderProgram();

    public:
        ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;

    public:
        bool isCompiled() const { return m_isCompiled; }
        /**
         * Метод запускает шейдерную программу.
         * */
        void use() const;
        void setInt(const std::string& name, const GLint value);

    private:
        // Поле показывает, собралась шейдерная программа или нет
        bool m_isCompiled = false;
        // Идентификатор шейдерной программы
        GLuint m_ID = 0;

    private:
        /**
         * Метод создает шейдер. В случае, если шейдер создать не получилось, в std::cerr будет
         * выведено сообщение об ошибке в исходном коде шейдера.
         * @param source исходный код создаваемого шейдера
         * @param shaderType тип создаваемого шейдера
         * @param shaderID в данный параметр будет записан идентификатор созданного шейдера
         * @return true - если шейдер был удачно создан, false - если шейдер создать не удалось
         * */
        bool createShader(const std::string& source, GLenum shaderType, GLuint& shaderID);
    };
}