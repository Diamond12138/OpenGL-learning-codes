#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include "utilities.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "01", sf::Style::Default, sf::ContextSettings(8, 8, 8, 4, 6, sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Debug, false));

    initGL(window);
    glViewport(0,0,640,480);
    GLuint program = loadProgram("shader/03.vert","shader/03.frag");
    GLuint vao = createVAO(generatePlane_2DLighting());
    GLuint texture = createTexture2D("resources/1.jpg");

    GLint uniform_angle = getUniformLocation(program,"u_angle");
    sf::Clock clock;

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
        glUniform1f(uniform_angle,glm::radians(clock.getElapsedTime().asSeconds() * 30.0f));
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D,texture);
        glDrawArrays(GL_TRIANGLES,0,12);
        glBindVertexArray(0);
        window.display();
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1,&vao);
    glDeleteTextures(1,&texture);
    return 0;
}