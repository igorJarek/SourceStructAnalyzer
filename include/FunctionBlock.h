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

        sf::Vector2u getSize()                              const   { return size; }
        void setWidth(float width);
        bool isContainsPoint(sf::Vector2f point);
        string getFunctionNameFromPoint(sf::Vector2f point);
        string getFunctionName()                            const   { return name.getString(); }

    private:
        class FunctionPositionInfo
        {
            public:
                FunctionPositionInfo(const string& functionName, float xStart, float xEnd)
                    :  m_functionName(functionName), m_xStart(xStart), m_xEnd(xEnd) { }

                string m_functionName;
                float m_xStart, m_xEnd;
        };

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        sf::Vector2u size;
        sf::Text absolutePath;
        sf::Text name;

        list<pair<list<FunctionPositionInfo>, TextExt>> rows;

        sf::RectangleShape border;
};

#endif // FUNCTIONBLOCK_H
