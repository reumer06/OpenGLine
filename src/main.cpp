#include "glad.h"
#include <GLFW/glfw3.h>
#include <print>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

const unsigned int SCR_WIDTH{800};
const unsigned int SCR_HEIGHT{800};

void framebuffer_size_callback(GLFWwindow *window, GLint width, GLint height);

void process_input(GLFWwindow *window);

int main()
{
    int lineNum, dashLength, flag;
    std::vector<float> vertices;

    if (!glfwInit()) {
        std::println("Failed to initialize GLFW");
        return -1;
    };
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window{glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OPENGLine",NULL,NULL)};
    if (window == NULL) {
        std::println("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::println("Failed to initialize GLAD");
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, GLint width, GLint height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window,GLFW_KEY_J) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

