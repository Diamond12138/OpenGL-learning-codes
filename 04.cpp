#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <array>
#include "utilities.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "01", sf::Style::Default, sf::ContextSettings(8, 8, 8, 4, 6, sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Debug, false));

    initGL(window);
    glViewport(0, 0, 640, 480);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint program = loadProgram("shader/04.vert", "shader/04.frag");
    const float size = 5;
    GLuint vao = createVAO(generatePlane(glm::vec2(size,size)));
    GLuint texture = createVolumeTexture(8, glm::vec4(0, 0, 1, 0.5));

    GLint uniform_projection = getUniformLocation(program, "u_projection");
    GLint uniform_view = getUniformLocation(program, "u_view");
    GLint uniform_model = getUniformLocation(program, "u_model");
    GLint uniform_camera_position = getUniformLocation(program, "u_camera_position");

    std::array<Transform, 6> planes;
    planes.at(0).translate = glm::vec3(0.0f, 0.0f, size * 0.5f);
    planes.at(0).rotate = glm::vec3(0.0f, 0.0f, 0.0f);

    planes.at(1).translate = glm::vec3(0.0f, 0.0f, -size * 0.5f);
    planes.at(1).rotate = glm::vec3(0.0f, 180.0f, 0.0f);

    planes.at(2).translate = glm::vec3(size * 0.5f, 0.0f, 0.0f);
    planes.at(2).rotate = glm::vec3(0.0f, 90.0f, 0.0f);

    planes.at(3).translate = glm::vec3(-size * 0.5f, 0.0f, 0.0f);
    planes.at(3).rotate = glm::vec3(0.0f, 270.0f, 0.0f);

    planes.at(4).translate = glm::vec3(0.0f, -size * 0.5f, 0.0f);
    planes.at(4).rotate = glm::vec3(-90.0f, 0.0f, 0.0f);

    planes.at(5).translate = glm::vec3(0.0f, size * 0.5f, 0.0f);
    planes.at(5).rotate = glm::vec3(90.0f, 0.0f, 0.0f);
    for (unsigned int i = 0; i < planes.size(); i++)
    {
        updateTransform(planes.at(i));
    }

    float distance = 1;

    View view;
    view.position = glm::vec3(distance, distance, distance);
    view.front = glm::vec3(0, 0, 0) - view.position;
    updateView(view);
    PerspectiveProjection projection;
    projection.aspect = 640.0f / 480.0f;
    updatePerspectiveProjection(projection);

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
                sf::Vector2u size = window.getSize();
                glViewport(0, 0, size.x, size.y);
            }
            else if (event.type == sf::Event::EventType::MouseWheelScrolled)
            {
                distance += event.mouseWheelScroll.delta * 0.1f;
                view.position = glm::vec3(distance, distance, distance);
                view.front = glm::vec3(0, 0, 0) - view.position;
                updateView(view);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glUseProgram(program);
        glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, &projection.matrix[0][0]);
        glUniformMatrix4fv(uniform_view, 1, GL_FALSE, &view.matrix[0][0]);
        glUniform3fv(uniform_camera_position, 1, glm::value_ptr(view.position));
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_3D, texture);
        for (unsigned int i = 0; i < planes.size(); i++)
        {
            glUniformMatrix4fv(uniform_model, 1, GL_FALSE, &planes.at(i).matrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glBindVertexArray(0);
        window.display();
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &texture);
    return 0;
}