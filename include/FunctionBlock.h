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
        FunctionBlock(RowedFile& rowedFile, const string& functionName, std::pair<int, int> range, std::list<unsigned int>& detectedFuntions);
        ~FunctionBlock();

        sf::Vector2u getSize() { return size; }

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        bool isDetectedFuncListContainsLine(unsigned int line);

        sf::Vector2u size;
        sf::Text titlePath;
        sf::Text searchingFunction;

        std::list<sf::Text> rows;
        sf::RectangleShape border;

        std::pair<int, int> fileRange;
        std::list<unsigned int> funtionLineDetectedList;
};

#endif // FUNCTIONBLOCK_H
