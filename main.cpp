#include <iostream>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include "utils.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "Text Rendering", sf::Style::Close, sf::ContextSettings(16, 8, 4, 4, 6, sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Debug, false));

    tr::flog.open("log.txt");
    if (tr::flog.fail())
    {
        std::cout << "could not open file:\"log.txt\"" << std::endl;
        return EXIT_FAILURE;
    }

    if (!tr::initGL())
        return EXIT_FAILURE;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint program = tr::loadProgramFromFile("resources/text.vert", "resources/text.frag");
    GLint uniform_color = glGetUniformLocation(program, "u_color");

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    std::map<char, tr::Glyph> glyphs;
    if (!tr::loadGlyphs("resources/simsun.ttc", 32, glyphs))
        return EXIT_FAILURE;

    // 创建VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);

    // 绑定
    glBindVertexArray(vao);
    GLuint vbo1 = tr::createVBO(
        {
            -1.0f,
            1.0f,

            -1.0f,
            -1.0f,

            1.0f,
            -1.0f,

            1.0f,
            1.0f,
        },
        0);
    GLuint vbo2 = tr::createVBO(
        {
            0.0f,
            0.0f,

            0.0f,
            1.0f,

            1.0f,
            1.0f,

            1.0f,
            0.0f,
        },
        1);

    // 恢复初始状态
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::string str = "";
    const float SCALE_RATE = 2.0f / 640.0f;

    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::EventType::Closed:
                window.close();
                break;

            case sf::Event::EventType::KeyPressed:
                /*if (event.key.code >= sf::Keyboard::Key::A && event.key.code <= sf::Keyboard::Key::Z)
                {
                    str += (event.key.shift ? 'A' : 'a') + event.key.code;
                }
                else if (event.key.code >= sf::Keyboard::Key::Num0 && event.key.code <= sf::Keyboard::Key::Num9)
                {
                    str += '0' + event.key.code - sf::Keyboard::Key::Num0;
                }
                else if (event.key.code >= sf::Keyboard::Key::Numpad0 && event.key.code <= sf::Keyboard::Key::Numpad9)
                {
                    str += '0' + event.key.code - sf::Keyboard::Key::Numpad0;
                }
                else if (event.key.code >= sf::Keyboard::Key::Enter)
                {
                    str += '\n';
                }
                else if (event.key.code == sf::Keyboard::Key::Backspace)
                {
                    if (str.length() > 0)
                        str.pop_back();
                }*/

                break;

            case sf::Event::EventType::TextEntered:
            {
                sf::Uint32 code = event.text.unicode;
                switch (code)
                {
                case '\b':
                    if (str.length() > 0)
                        str.pop_back();
                    break;

                case '\n':
                    str += '\n';
                    break;

                default:
                    str += code;
                    break;
                }
                tr::flog << "str:" << str << std::endl;
            }
            break;

            default:
                break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        glUseProgram(program);
        glUniform4f(uniform_color, 1.0f, 1.0f, 1.0f, 1.0f);
        float x = -1.0f;
        float y = 0.9f;
        for (size_t i = 0; i < str.length(); i++)
        {
            const char &ch = str.at(i);
            tr::Glyph glyph = glyphs.at(ch);

            if (ch == 13)
            {
                y -= 32.0f * SCALE_RATE;
                x = -1.0f;
                continue;
            }

            GLfloat xpos = x + glyph.bearing.x * SCALE_RATE;
            GLfloat ypos = y - (glyph.size.y - glyph.bearing.y) * SCALE_RATE;

            GLfloat w = glyph.size.x * SCALE_RATE;
            GLfloat h = glyph.size.y * SCALE_RATE;

            GLfloat vertices[4][2] =
                {
                    {xpos, ypos + h},
                    {xpos, ypos + 0},
                    {xpos + w, ypos + 0},
                    {xpos + w, ypos + h},
                };

            glBindTexture(GL_TEXTURE_2D, glyph.texture);

            // 更新VBO
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo1);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // 绘制
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindVertexArray(0);

            x += glyph.advance.x * SCALE_RATE;
        }

        window.display();
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo1);
    glDeleteBuffers(1, &vbo2);
    for (size_t i = 0; i < 128; i++)
    {
        glDeleteTextures(1, &glyphs.at(i).texture);
    }

    tr::flog.close();

    return EXIT_SUCCESS;
}