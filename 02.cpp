#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include "utilities.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "01", sf::Style::Default, sf::ContextSettings(8, 8, 8, 4, 6, sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Debug, false));

    initGL(window);
    
    sf::Vector2u size = window.getSize();
    glViewport(0, 0, size.x, size.y);
    GLuint program = loadProgram("shader/02.vert", "shader/02.frag");
    GLuint vao = createVAO(generatePlane());
    GLuint texture = createTexture2D("resources/1.jpg");

    Transform transform;
    updateTransform(transform);
    View view;
    view.position = glm::vec3(1,1,1);
    view.front = glm::vec3(-1,-1,-1);
    updateView(view);
    PerspectiveProjection projection;
    projection.aspect = float(size.x) / float(size.y);
    updatePerspectiveProjection(projection);

    GLint uniform_model = getUniformLocation(program, "u_model");
    GLint uniform_view = getUniformLocation(program, "u_view");
    GLint uniform_projection = getUniformLocation(program, "u_projection");

    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (needQuit(event))
            {
                window.close();
            }
            else if (event.type == sf::Event::EventType::Resized)
            {
                size = window.getSize();
                glViewport(0, 0, size.x, size.y);
                projection.aspect = float(size.x) / float(size.y);
                updatePerspectiveProjection(projection);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, &projection.matrix[0][0]);
        glUniformMatrix4fv(uniform_view, 1, GL_FALSE, &view.matrix[0][0]);
        glUniformMatrix4fv(uniform_model, 1, GL_FALSE, &transform.matrix[0][0]);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        window.display();
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &texture);
    return 0;
}