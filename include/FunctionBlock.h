#ifndef FUNCTIONBLOCK_H
#define FUNCTIONBLOCK_H

#include <SFML/Graphics.hpp>
#include <list>
#include <utility>

#include <RowedFile.h>
#include <Resource.h>

class FunctionBlock : public sf::Drawable, public sf::Transformable
{
    public:
        FunctionBlock(RowedFile& rowedFile, std::pair<int, int> range);
        ~FunctionBlock();

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        sf::Text titlePath;

        std::list<sf::Text> rows;
        sf::RectangleShape border;
};

#endif // FUNCTIONBLOCK_H
