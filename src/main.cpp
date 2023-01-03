#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int g_windowSizeX = 640;
int g_windowSizeY = 480;

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

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (! gladLoadGL()) {
        std::cout << "Can't load GLAD" << std::endl;
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

    /* Loop until the user closes the window */
    while (! glfwWindowShouldClose(pWindow)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}