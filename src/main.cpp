#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// Координаты вершин треугольника
GLfloat point[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5, 0.0f
};

// Цвета вершин треугольника
GLfloat color[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

const GLchar* vertexShader {
    "#version 410 core\n"
    "layout (location = 0) in vec3 vertexPosition;\n"
    "layout (location = 1) in vec3 vertexColor;\n"
    "out vec3 color;\n"
    "void main() {\n"
    "    color = vertexColor;\n"
    "    gl_Position = vec4(vertexPosition, 1.0);\n"
    "}\0"
};

const GLchar* fragmentShader {
    "#version 410 core\n"
    "in vec3 color;\n"
    "out vec4 fragColor;\n"
    "void main() {\n"
    "    fragColor = vec4(color, 1.0);\n"
    "}\0"
};

// Размер окна по координате Х (ширина)
int g_windowSizeX = 640;
// Размер окна по координате У (высота)
int g_windowSizeY = 480;

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

bool checkShaderErrors(GLuint shader) {
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (! success) {
	    glGetShaderInfoLog(shader, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

bool checkProgramErrors(GLuint program) {
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (! success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

int main() {
    /* Initialize the library */
    if (!glfwInit()) {
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
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY,
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

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, nullptr);
    glCompileShader(vs);
    if (! checkShaderErrors(vs)) {
        std::cout << "Vertex" << std::endl;
        glfwTerminate();
        return -1;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, nullptr);
    glCompileShader(fs);
    if (! checkShaderErrors(fs)) {
        std::cout << "Fragment" << std::endl;
        glfwTerminate();
        return -1;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    if (! checkProgramErrors(shaderProgram)) {
        std::cout << "Linking" << std::endl;
        return -1;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint pointsVBO = 0;
    glGenBuffers(1, &pointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

    GLuint colorVBO = 0;
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    /* Loop until the user closes the window */
    while (! glfwWindowShouldClose(pWindow)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}