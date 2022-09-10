#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include "utilities.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "01", sf::Style::Default, sf::ContextSettings(8, 8, 8, 4, 6, sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Debug, false));

    initGL(window);
    glViewport(0,0,640,480);
    GLuint program = loadProgram("shader/01.vert","shader/01.frag");
    GLuint vao = createVAO(generatePlane());
    GLuint texture = createTexture2D("resources/1.jpg");

    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (needQuit(event))
            {
                window.close();
            }
            else if(event.type == sf::Event::EventType::Resized)
            {
                sf::Vector2u size = window.getSize();
                glViewport(0,0,size.x,size.y);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D,texture);
        glDrawArrays(GL_TRIANGLES,0,6);
        glBindVertexArray(0);
        window.display();
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1,&vao);
    glDeleteTextures(1,&texture);
    return 0;
}