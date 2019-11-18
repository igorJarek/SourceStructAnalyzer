#ifndef CONNECTIONLINE_H
#define CONNECTIONLINE_H

#include <SFML/Graphics.hpp>
#include <list>
#include <cmath>

#include <../config.h>

class ConnectionLine : public sf::Drawable, public sf::Transformable
{
    public:
        ConnectionLine(sf::Vector2i pos1, sf::Vector2i pos2, sf::Vector2i pos3, sf::Vector2i pos4);
        ~ConnectionLine();

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::RectangleShape lines[3];
};

#endif // CONNECTIONLINE_H
