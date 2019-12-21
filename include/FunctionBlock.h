#ifndef FUNCTIONBLOCK_H
#define FUNCTIONBLOCK_H

#include <SFML/Graphics.hpp>
#include <list>

#include <ParsedFile.h>
#include <RowedFile.h>
#include <Resource.h>
#include <Typedefs.h>
#include <TextExt.h>

class FunctionBlock : public sf::Drawable, public sf::Transformable
{
    public:
        FunctionBlock(ParsedFilePtr parsedFilePtr, const string& functionName);
        ~FunctionBlock();

        sf::Vector2u getSize() const { return size; }

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        sf::Vector2u size;
        sf::Text absolutePath;
        sf::Text name;

        list<TextExt> rows;
        sf::RectangleShape border;
};

#endif // FUNCTIONBLOCK_H
