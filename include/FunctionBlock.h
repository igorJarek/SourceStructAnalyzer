#ifndef FUNCTIONBLOCK_H
#define FUNCTIONBLOCK_H

#include <SFML/Graphics.hpp>
#include <list>

#include <RowedFile.h>

extern sf::Font font;

class FunctionBlock : public sf::Drawable, public sf::Transformable
{
    public:
        FunctionBlock(RowedFile& rowedFile, int charSize = 8, int padding = 6);
        ~FunctionBlock();

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        std::list<sf::Text> rows;
        sf::RectangleShape border;
};

#endif // FUNCTIONBLOCK_H