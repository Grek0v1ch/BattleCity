#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"

// Координаты вершин треугольника
GLfloat point[] = {
    0.0f, 50.0f, 0.0f,
    50.0f, -50.0f, 0.0f,
    -50.0f, -50.0, 0.0f
};

// Цвета вершин треугольника
GLfloat color[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

// Координаты текстуры
GLfloat texCoord[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

glm::ivec2 g_windowSize(640, 480);


/**
 * Функция-callback для обработки изменения размеров окна
 * @param pWindow  указатель на окно
 * @param width новое значение ширины окна
 * @param height новое значение высоты окна
 * */
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height);

/**
 * Функция-callback для обработки нажатия клавиш
 * @param pWindow  указатель на окно
 * @param key код клавиши
 * @param action действие с клавишей
 * */
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode);

int main(int argc, char** argv) {
    /* Initialize the library */
    if (! glfwInit()) {
        std::cout << "GLFW not initialized" << std::endl;
        return -1;
    }

    // Установка версии OpenGL и Core-профиля
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y,
                                           "Battle City", nullptr, nullptr);
    if (! pWindow) {
        std::cout << "Window not initialized" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (! gladLoadGL()) {
        std::cout << "Can't load GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
#ifndef __APPLE__
    std::cout << "Renderer: " << glGetString(GL_RENDER) << std::endl;
#else
    std::cout << "Renderer: Apple M1" << std::endl;
#endif
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(1, 1, 0 , 1);

    // Создаем область видимости, чтобы удалить шейдеры до удаления контекста OpenGL
    {
        ResourceManager resourceManager(argv[0]);
        std::shared_ptr<Renderer::ShaderProgram> pDefaultShaderProgram;
        try {
            pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader",
                                                                 "res/shaders/vertex.txt",
                                                                 "res/shaders/fragment.txt");
        } catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            return -1;
        }
        if (!pDefaultShaderProgram) {
            std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
            return -1;
        }

        auto pTex = resourceManager.loadTexture("Default Texture", "res/textures/map_16x16.png");

        GLuint pointsVBO = 0;
        glGenBuffers(1, &pointsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

        GLuint colorVBO = 0;
        glGenBuffers(1, &colorVBO);
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

        GLuint texCoordVBO = 0;
        glGenBuffers(1, &texCoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

        GLuint VAO = 0;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDefaultShaderProgram->use();
        try {
            pDefaultShaderProgram->setUniform("tex", 0);
        } catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            return -1;
        }

        glm::mat4 modelMatrix_1(1.0f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100, 50, 0));
        glm::mat4 projectionMatrix = glm::ortho(0.0f,
                                                static_cast<float>(g_windowSize.x),
                                                0.0f, static_cast<float>(g_windowSize.y),
                                                -100.0f, 100.0f);
        glm::mat4 modelMatrix_2(1.0f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590, 50, 0));

        try {
            pDefaultShaderProgram->setUniform("projectionMat", projectionMatrix);
        } catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            return -1;
        }

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow)) {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(VAO);
            pTex->bind();

            try {
                pDefaultShaderProgram->setUniform("modelMat", modelMatrix_1);
            } catch (const std::exception& ex) {
                std::cerr << ex.what() << std::endl;
                return -1;
            }
            glDrawArrays(GL_TRIANGLES, 0, 3);

            try {
                pDefaultShaderProgram->setUniform("modelMat", modelMatrix_2);
            } catch (const std::exception& ex) {
                std::cerr << ex.what() << std::endl;
                return -1;
            }
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glBindVertexArray(0);

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, g_windowSize.x, g_windowSize.y);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}