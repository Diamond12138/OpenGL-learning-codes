#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glew.h>


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(640,480,"",nullptr,nullptr);
    glfwMakeContextCurrent(window);
    glewInit();
    glfwTerminate();
    return 0;
}