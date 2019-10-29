#include <SFML/Graphics.hpp>
#include <iostream>

#include "version.h"

#include <RowedFile.h>
#include <FunctionBlock.h>

int main()
{
    sf::VideoMode vMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(vMode.width, vMode.height), "Source Structure Analyzer " + VERSION_STRING);
    window.setFramerateLimit(60);

    RowedFile rowedFile("main.tests");
    FunctionBlock functionalBlock(rowedFile);
    functionalBlock.setPosition(10, 10);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Up)
                {

                }
                else if(event.key.code == sf::Keyboard::Right)
                {

                }
                else if(event.key.code == sf::Keyboard::Down)
                {

                }
                else if(event.key.code == sf::Keyboard::Left)
                {

                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(functionalBlock);
        window.display();
    }

    return 0;
}
