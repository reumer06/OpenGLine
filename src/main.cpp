#include "glad.h"
#include <GLFW/glfw3.h>
#include <print>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <random>


const unsigned int SCR_WIDTH{800};
const unsigned int SCR_HEIGHT{800};

void framebuffer_size_callback(GLFWwindow *window, GLint width, GLint height);

void process_input(GLFWwindow *window);

void glfwErrorCallback(int error, const char *description);

std::string readFile(const char *path);

void basicAlg(int dashLength, int xStart, int yStart, int xEnd, int yEnd, std::vector<float> &vertices);

int main()
{
    int lineNum, dashLength, flag;
    std::vector<float> vertices;

    std::print("Enter the number of lines: ");
    std::cin >> lineNum;

    std::print("Enter the dash length: ");
    std::cin >> dashLength;

    std::print("Choose your algorithm: (0 for Basic, 1 for Bresenham): ");
    std::cin >> flag;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 600);

    glfwSetErrorCallback(glfwErrorCallback);
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

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glPointSize(1.0f); // set diameter of point

    GLint success;
    char infoLog[512];

    GLuint vertexShader{glCreateShader(GL_VERTEX_SHADER)};
    std::string vertexShaderSourceStr{readFile("shaders/vertexshader.vert")};
    const char *vertexShaderSource{vertexShaderSourceStr.c_str()};
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::println("VERTEX SHADER COMPILATION FAILED: {}", infoLog);
    }

    GLuint fragmentShader(glCreateShader(GL_FRAGMENT_SHADER));
    std::string fragmentShaderSourceStr{readFile("shaders/fragmentshader.frag")};
    const char *fragmentShaderSource{fragmentShaderSourceStr.c_str()};
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::println("FRAGMENT SHADER COMPILATION FAILED: {}", infoLog);
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram,GL_LINK_STATUS, &success);
    if (!success) {
        std::println("PROGRAM LINKING FAILED: {}", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 2,GL_FLOAT,GL_FALSE, 2 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    vertices.clear();
    for (int i = 0; i < lineNum; ++i) {
        // generate random start and end points.
        int xStart = dist(gen);
        int yStart = dist(gen);
        int xEnd = dist(gen);
        int yEnd = dist(gen);

        std::println("Line {}: (x0, y0) = ({}, {}); \n(x1, y1) = ({}, {})", i + 1, xStart, yStart, xEnd, yEnd);
        // if (flag) {
        // } else {
        // };
    }

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
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

void glfwErrorCallback(int error, const char *description)
{
    std::println("GLFW Error: {} \n Description: {}", error, description);
}


std::string readFile(const char *path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::println("Error opening file");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string content{buffer.str()};
    return content;
}

void basicAlg(int dashLength, int xStart, int yStart, int xEnd, int yEnd, std::vector<float> &vertices)
{
    int dx = abs(xEnd - xStart);
    int dy = abs(yEnd - yStart);
    int x, y;
    float m, xInc, yInc;

    if (dashLength == 0) {
        // Solid Line
        if (dx >= dy) {
            // shallow line
            if (xStart <= xEnd) {
                // if line is moving from the left to right
                x = xStart;
                y = yStart;
            } else {
                // swap the points of right to left
                x = xEnd;
                y = yEnd;
                xEnd = xStart;
                yEnd = yStart;
            }
            m = (float) dy / (float) dx;
            for (int i = 0; i <= dx; ++i) {
                xInc = x + i;
                yInc = (y > yEnd)
                           ? (
                               -m * i + y
                           )
                           : (m * i + y);
                vertices.push_back(xInc);
                vertices.push_back((float) (int) yInc);
            }
        } else {
            // steep line
            if (yStart <= yEnd) {
                x = xStart;
                y = yStart;
            } else {
                x = xEnd;
                y = yEnd;
                xEnd = xStart;
                yEnd = yStart;
            }
            m = (float) dx / (float) dy;
            for (int i = 0; i <= dy; i++) {
                yInc = y + i;
                xInc = (x > xEnd) ? (-m * i + x) : (m * i + x);
                vertices.push_back((float) ((int) xInc));
                vertices.push_back(yInc);
            }
        }
    } else {
        // dashed line
        int dashed = 0, dot = 0;
        if (dx >= dy) {
            if (xStart <= xEnd) {
                x = xStart;
                y = yStart;
            } else {
                x = xEnd;
                y = yEnd;
                yEnd = yStart;
                xEnd = xStart;
            }
            m = (float) dy / (float) dx;
            for (int i = 0; i <= dx; ++i) {
                xInc = x + i;
                yInc = (y > yEnd) ? (-m * i + y) : (m * i + y);
                if (!dashed) {
                    vertices.push_back(xInc);
                    vertices.push_back((float) (int) yInc);
                }
                dot++;
                if (dot % dashLength == 0) {
                    dashed != dashed;
                }
            }
        } else {
            if (yStart <= yEnd) {
                x = xStart;
                y = yStart;
            } else {
                x = xEnd;
                y = yEnd;
                xEnd = xStart;
                yEnd = yStart;
            }
            m = (float) dx / (float) dy;
            for (int i = 0; i <= dy; ++i) {
                yInc = y + i;
                xInc = (x > xEnd) ? (-m * i + x) : (m * i + x);
                if (!dashed) {
                    vertices.push_back(yInc);
                    vertices.push_back((float) (int) xInc);
                }
                dot++;
                if (dot % dashLength == 0) {
                    dashed != dashed;
                }
            }
        }
    }
}

