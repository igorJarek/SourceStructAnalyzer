#include <FunctionBlock.h>

FunctionBlock::FunctionBlock(RowedFile& rowedFile, std::pair<int, int> range)
{
    rowedFile.resetFileReadedPtr();
    setOrigin(-(FB_PADDING+FB_BORDER_THICKNESS), -(FB_PADDING+FB_BORDER_THICKNESS));
    int startYPos = 0;
    int maxWidth = 0;
    int line = 0;

    titlePath.setFont(Resource::instance().getFuncBlockFont());
    titlePath.setString(rowedFile.getPath());
    titlePath.setCharacterSize(FB_CHAR_SIZE + 3);
    titlePath.setFillColor(sf::Color::Red);
    titlePath.setPosition(0, startYPos);

    sf::FloatRect fr = titlePath.getGlobalBounds();
    if(fr.width > maxWidth) maxWidth = fr.width;

    startYPos += fr.height + 16; /* additional space between rows*/

    while(!rowedFile.isEOF())
    {
        line++;
        if(line < range.first)
            continue;
        else if(line > range.second)
            break;

        string lineNumer = to_string(line) + ") ";
        sf::Text text;
        text.setFont(Resource::instance().getFuncBlockFont());
        text.setString(lineNumer + rowedFile.getNextRow());
        text.setCharacterSize(FB_CHAR_SIZE);
        text.setFillColor(sf::Color::Black);
        text.setPosition(0, startYPos);

        fr = text.getGlobalBounds();
        if(fr.width > maxWidth) maxWidth = fr.width;

        startYPos += fr.height + 4; /* additional space between rows*/

        rows.push_front(text);
    }

    border.setSize(sf::Vector2f(maxWidth + (2*FB_PADDING), startYPos + (2*FB_PADDING)));
    border.setPosition(-FB_PADDING, -FB_PADDING);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(FB_BORDER_THICKNESS);
    border.setOutlineColor(sf::Color(128,0,0));

    size.x = maxWidth + (2*FB_PADDING) + (2*FB_BORDER_THICKNESS);
    size.y = startYPos + (2*FB_PADDING) + (2*FB_BORDER_THICKNESS);
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
