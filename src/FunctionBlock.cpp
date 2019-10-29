#include <FunctionBlock.h>

FunctionBlock::FunctionBlock(RowedFile& rowedFile, int charSize, int padding)
{
    int startYPos = 0;
    int maxWidth = 0;
    int lineNumber = 1;
    while(!rowedFile.isEOF())
    {
        sf::Text text;
        text.setFont(Resource::instance().getFuncBlockFont());
        text.setString(rowedFile.getNextRow());
        text.setCharacterSize(charSize);
        text.setFillColor(sf::Color::Black);
        text.setPosition(0, startYPos);

        sf::FloatRect fr = text.getGlobalBounds();
        if(fr.width > maxWidth) maxWidth = fr.width;

        startYPos += fr.height + 4; /* additional space between rows*/

        rows.push_front(text);
    }

    border.setSize(sf::Vector2f(maxWidth + (2*padding), startYPos + (2*padding)));
    border.setPosition(-padding, -padding);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(4.f);
    border.setOutlineColor(sf::Color(128,0,0));
}

FunctionBlock::~FunctionBlock()
{
    //dtor
}

void FunctionBlock::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for(sf::Text text : rows)
        target.draw(text, states);

    target.draw(border, states);
}
