#include <FunctionBlock.h>

FunctionBlock::FunctionBlock(RowedFile& rowedFile, int charSize, int padding, int borderThickness)
{
    setOrigin(-(padding+borderThickness), -(padding+borderThickness));
    int startYPos = 0;
    int maxWidth = 0;
    int line = 1;

    titlePath.setFont(Resource::instance().getFuncBlockFont());
    titlePath.setString(rowedFile.getPath());
    titlePath.setCharacterSize(charSize + 3);
    titlePath.setFillColor(sf::Color::Red);
    titlePath.setPosition(0, startYPos);

    sf::FloatRect fr = titlePath.getGlobalBounds();
    if(fr.width > maxWidth) maxWidth = fr.width;

    startYPos += fr.height + 16; /* additional space between rows*/

    while(!rowedFile.isEOF())
    {
        string lineNumer = to_string(line++) + ". ";
        sf::Text text;
        text.setFont(Resource::instance().getFuncBlockFont());
        text.setString(lineNumer + rowedFile.getNextRow());
        text.setCharacterSize(charSize);
        text.setFillColor(sf::Color::Black);
        text.setPosition(0, startYPos);

        fr = text.getGlobalBounds();
        if(fr.width > maxWidth) maxWidth = fr.width;

        startYPos += fr.height + 4; /* additional space between rows*/

        rows.push_front(text);
    }

    border.setSize(sf::Vector2f(maxWidth + (2*padding), startYPos + (2*padding)));
    border.setPosition(-padding, -padding);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(borderThickness);
    border.setOutlineColor(sf::Color(128,0,0));
}

FunctionBlock::~FunctionBlock()
{
    //dtor
}

void FunctionBlock::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(titlePath, states);

    for(sf::Text text : rows)
        target.draw(text, states);

    target.draw(border, states);
}
