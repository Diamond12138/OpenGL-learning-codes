#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include "utilities.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(100, 100), "01", sf::Style::Fullscreen, sf::ContextSettings(8, 8, 8, 4, 6, sf::ContextSettings::Attribute::Core | sf::ContextSettings::Attribute::Debug, true));

    initGL(window);

    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Key::Escape:
                    window.close();
                    break;
                default:
                    break;
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        window.display();
    }
}