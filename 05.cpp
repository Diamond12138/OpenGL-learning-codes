#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include "utilities.hpp"

struct OrthoCamera : public Camera
{
    glm::mat4 projection;
    glm::mat4 view;

    virtual glm::mat4 getProjection()
    {
        return projection;
    }

    virtual glm::mat4 getView()
    {
        return view;
    }
};

const GLint uniform_enable_texture = UniformLocation::COUNT;
const GLint uniform_color = UniformLocation::COUNT + 1;
const GLint uniform_shadow = UniformLocation::COUNT + 2;
const GLint uniform_texture = UniformLocation::COUNT + 3;
const GLint uniform_light_projection = UniformLocation::COUNT + 4;
const GLint uniform_light_view = UniformLocation::COUNT + 5;

void gen_shadow(Camera &camera, RenderableObject plane1, RenderableObject plane2);
void draw_texture(RenderableObject plane3, DirectionalLightShadow shadow);
void draw_shadow(Camera &camera, RenderableObject plane1, RenderableObject plane2, GLuint texture, DirectionalLightShadow shadow);

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "05", sf::Style::Default, sf::ContextSettings(16, 8, 4, 4, 6, sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Debug, false));
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(1);

    initGL(window);
    sf::Vector2u size = window.getSize();
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, size.x, size.y);

    Model model = generatePlane();
    GLuint vao = createVAO(model);
    Model model2 = generatePlane(glm::vec2(2, 2));
    GLuint vao2 = createVAO(model2);
    Model model3 = generateAxis();
    GLuint vao3 = createVAO(model3);

    GLuint texture = createTexture2D("resources/1.jpg");

    GLuint program_gen_shadow = loadProgram("shader/05_gen_shadow.vert", "shader/05_gen_shadow.frag");
    GLuint program_draw_shadow = loadProgram("shader/05_draw_shadow.vert", "shader/05_draw_shadow.frag");
    GLuint program_draw_texture = loadProgram("shader/05_draw_texture.vert", "shader/05_draw_texture.frag");
    GLuint program_axis = loadProgram("shader/05_axis.vert", "shader/05_axis.frag");

    DirectionalLightShadow shadow = createDLShadow(glm::vec3(10, 20, 30), glm::vec3(-10, -20, -30), 50, 1024, 1024, glm::vec2(5,5));

    RenderableObject plane1(vao, 6);
    plane1.rotate = glm::vec3(90, 0, 0);
    plane1.scale = glm::vec3(5, 5, 5);
    updateTransform(plane1);

    RenderableObject plane2(vao, 6);
    plane2.translate = glm::vec3(0, 1, 0);
    updateTransform(plane2);

    RenderableObject plane3(vao2, 6);
    plane3.rotate = glm::vec3(180,0,0);
    updateTransform(plane3);

    PerspectiveCamera camera;
    camera.position = glm::vec3(-2, 2, 2);
    //camera.position = glm::vec3(2, 4, 6);
    camera.front = glm::vec3(0, 0, 0) - camera.position;
    camera.aspect = float(size.x) / float(size.y);
    updateView(camera);
    updatePerspectiveProjection(camera);

    OrthoCamera light_camera;
    light_camera.projection = shadow.projection.matrix;
    light_camera.view = shadow.view.matrix;

    bool is_draw_shadow = true;
    sf::Clock clock;
    float last_display = clock.getElapsedTime().asSeconds();

    auto draw = [&](float current_display)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, shadow.FBO);
        glViewport(0, 0, 1024, 1024);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glUseProgram(program_gen_shadow);
        gen_shadow(light_camera, plane1, plane2);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, size.x, size.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0, 0.5, 0.9, 1.0);

        const glm::vec4 color_red = glm::vec4(1, 0, 0, 1);
        const glm::vec4 color_green = glm::vec4(0, 1, 0, 1);
        const glm::vec4 color_blue = glm::vec4(0, 0, 1, 1);
        glUseProgram(program_axis);
        glUniformMatrix4fv(UniformLocation::PROJECTION, 1, GL_FALSE, &camera.getProjection()[0][0]);
        glUniformMatrix4fv(UniformLocation::VIEW, 1, GL_FALSE, &camera.getView()[0][0]);
        glBindVertexArray(vao3);
        glUniform4fv(UniformLocation::COLOR, 1, glm::value_ptr(color_red));
        glDrawArrays(GL_LINES, 0, 2);
        glUniform4fv(UniformLocation::COLOR, 1, glm::value_ptr(color_green));
        glDrawArrays(GL_LINES, 2, 2);
        glUniform4fv(UniformLocation::COLOR, 1, glm::value_ptr(color_blue));
        glDrawArrays(GL_LINES, 4, 2);
        glBindVertexArray(0);

        if (is_draw_shadow)
        {
            glUseProgram(program_draw_shadow);
            draw_shadow(camera, plane1, plane2, texture, shadow);
        }
        else
        {
            glUseProgram(program_draw_texture);
            draw_texture(plane3, shadow);
        }

        window.display();
        last_display = current_display;
    };

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
                camera.aspect = float(size.x) / float(size.y);
                updatePerspectiveProjection(camera);
                draw(clock.getElapsedTime().asSeconds());
            }
            else if (event.type == sf::Event::EventType::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Key::D)
                {
                    is_draw_shadow = !is_draw_shadow;
                    draw(clock.getElapsedTime().asSeconds());
                }
            }
        }

        float current_display = clock.getElapsedTime().asSeconds();
        if (current_display - last_display > 1)
        {
            draw(current_display);
        }
    }

    glDeleteProgram(program_gen_shadow);
    glDeleteProgram(program_draw_texture);
    glDeleteProgram(program_draw_shadow);
    glDeleteProgram(program_axis);
    glDeleteVertexArrays(1, &vao);
    glDeleteVertexArrays(1, &vao2);
    glDeleteVertexArrays(1, &vao3);
    glDeleteTextures(1, &texture);
    deleteShadow(shadow);
    return 0;
}

void gen_shadow(Camera &camera, RenderableObject plane1, RenderableObject plane2)
{
    glUniformMatrix4fv(UniformLocation::PROJECTION, 1, GL_FALSE, &camera.getProjection()[0][0]);
    glUniformMatrix4fv(UniformLocation::VIEW, 1, GL_FALSE, &camera.getView()[0][0]);

    glBindVertexArray(plane1.VAO);
    glUniformMatrix4fv(UniformLocation::MODEL, 1, GL_FALSE, &plane1.getModel()[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUniformMatrix4fv(UniformLocation::MODEL, 1, GL_FALSE, &plane2.getModel()[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void draw_texture(RenderableObject plane3, DirectionalLightShadow shadow)
{
    glUniformMatrix4fv(UniformLocation::MODEL, 1, GL_FALSE, &plane3.getModel()[0][0]);
    glBindVertexArray(plane3.VAO);
    glBindTexture(GL_TEXTURE_2D, shadow.texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void draw_shadow(Camera &camera, RenderableObject plane1, RenderableObject plane2, GLuint texture, DirectionalLightShadow shadow)
{
    glUniformMatrix4fv(UniformLocation::PROJECTION, 1, GL_FALSE, &camera.getProjection()[0][0]);
    glUniformMatrix4fv(UniformLocation::VIEW, 1, GL_FALSE, &camera.getView()[0][0]);

    glBindVertexArray(plane1.VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadow.texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(uniform_shadow, 0);
    glUniform1i(uniform_texture, 1);

    glUniformMatrix4fv(uniform_light_projection, 1, GL_FALSE, &shadow.projection.matrix[0][0]);
    glUniformMatrix4fv(uniform_light_view, 1, GL_FALSE, &shadow.view.matrix[0][0]);

    glUniform1i(uniform_enable_texture, GL_TRUE);
    glUniformMatrix4fv(UniformLocation::MODEL, 1, GL_FALSE, &plane2.getModel()[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    const glm::vec4 color(0.5, 0.5, 0.5, 1);
    glUniform1i(uniform_enable_texture, GL_FALSE);
    glUniform4fv(uniform_color, 1, glm::value_ptr(color));
    glUniformMatrix4fv(UniformLocation::MODEL, 1, GL_FALSE, &plane1.getModel()[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}