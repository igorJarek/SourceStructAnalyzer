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
        sf::Vector2u getCenterOfEdgePos() { return {getPosition().x, getPosition().y + (getSize().y / 2)}; }
        unsigned int getDetectedFunctionAmount() const { return funtionLineDetectedList.size(); }
        std::list<unsigned int> getDetectedFunctionList() const { return funtionLineDetectedList; }
        sf::Vector2u getLineEndPos(int functionLine);

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        sf::Vector2u size;
        sf::Text titlePath;
        sf::Text searchingFunction;

        std::list<sf::Text> rows;
        sf::RectangleShape border;

        std::pair<int, int> fileRange;
        std::list<unsigned int> funtionLineDetectedList;
};

#endif // FUNCTIONBLOCK_H
