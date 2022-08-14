#include <iostream>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>

const char *vsource =
    "#version 330 core\n"
    "layout(location=0)in vec2 apos;"
    "void main(){gl_Position = vec4(apos,0.0,1.0);}";
const char *fsource =
    "#version 330 core\n"
    "out vec4 ocol;"
    "void main(){ocol = vec4(0.0,1.0,1.0,1.0);}";

const float ver[] =
    {
        0, 1,
        -1, -1,
        1, -1
    };

void debug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cout << "GL Error" << std::endl;
    std::cout << message << std::endl;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "", sf::Style::Close, sf::ContextSettings(8, 8, 8, 3, 3));
    window.setActive(true);

    std::cout << (glewInit() == GLEW_OK ? "success" : "failure") << std::endl;
    glDebugMessageCallback(debug,nullptr);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs, 1, &vsource, nullptr);
    glShaderSource(fs, 1, &fsource, nullptr);
    glCompileShader(vs);
    glCompileShader(fs);
    GLuint p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(ver),ver,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2 * sizeof(float),nullptr);
    glBindVertexArray(0);
    glDeleteBuffers(1,&vbo);

    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        //glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(p);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        window.display();
    }

    glDeleteProgram(p);
    glDeleteVertexArrays(1, &vao);
    return 0;
}