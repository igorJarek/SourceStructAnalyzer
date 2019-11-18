#include "ConnectionLine.h"

ConnectionLine::ConnectionLine(sf::Vector2i pos1, sf::Vector2i pos2, sf::Vector2i pos3, sf::Vector2i pos4)
{
    lines[0].setSize(sf::Vector2f{static_cast<float>(pos2.x - pos1.x) + 1, LINE_THICKNESS});
    lines[0].setOrigin(0, std::floor(LINE_THICKNESS / 2.0));
    lines[0].setFillColor(sf::Color::Red);
    lines[0].setPosition(static_cast<float>(pos1.x), static_cast<float>(pos1.y));

    lines[1].setSize(sf::Vector2f{LINE_THICKNESS, static_cast<float>(pos3.y - pos2.y) + 1});
    lines[1].setOrigin(std::floor(LINE_THICKNESS / 2.0), 0);
    lines[1].setFillColor(sf::Color::Red);
    lines[1].setPosition(static_cast<float>(pos2.x), static_cast<float>(pos2.y));

    lines[2].setSize(sf::Vector2f{static_cast<float>(pos4.x - pos3.x) + 1, LINE_THICKNESS});
    lines[2].setOrigin(0, std::floor(LINE_THICKNESS / 2.0));
    lines[2].setFillColor(sf::Color::Red);
    lines[2].setPosition(static_cast<float>(pos3.x), static_cast<float>(pos3.y));
}

ConnectionLine::~ConnectionLine()
{
    //dtor
}

void ConnectionLine::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(lines[0], states);
    target.draw(lines[1], states);
    target.draw(lines[2], states);
}
