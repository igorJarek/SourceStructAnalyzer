#include <FunctionBlock.h>

FunctionBlock::FunctionBlock(RowedFile& rowedFile, const string& functionName, Pos range, std::list<unsigned int>& detectedFuntions)
{
    fileRange = range;
    funtionLineDetectedList = detectedFuntions;

    setOrigin(-(FB_PADDING+FB_BORDER_THICKNESS), -(FB_PADDING+FB_BORDER_THICKNESS));
    uint32_t startYPos = 0;
    uint32_t maxWidth = 0;
    uint32_t currentLine = 0;

    searchingFunction.setFont(Resource::instance().getFuncBlockFont());
    searchingFunction.setString(functionName);
    searchingFunction.setCharacterSize(FB_CHAR_SIZE + 4);
    searchingFunction.setFillColor(sf::Color(0, 128, 64));
    searchingFunction.setPosition(0, startYPos);
    //searchingFunction.setStyle(sf::Text::Bold);

    sf::FloatRect fr = searchingFunction.getGlobalBounds();
    if(fr.width > maxWidth) maxWidth = fr.width;

    startYPos += fr.height + 10; /* additional space between rows*/

    titlePath.setFont(Resource::instance().getFuncBlockFont());
    titlePath.setString(rowedFile.getPath());
    titlePath.setCharacterSize(FB_CHAR_SIZE + 2);
    titlePath.setFillColor(sf::Color::Red);
    titlePath.setPosition(0, startYPos);

    fr = titlePath.getGlobalBounds();
    if(fr.width > maxWidth) maxWidth = fr.width;

    startYPos += fr.height + 16; /* additional space between rows*/

    string currentLineString;
    while(!rowedFile.isEOF())
    {
        currentLine++;
        currentLineString = rowedFile.getNextRow();
        if(currentLine < range.first)
            continue;
        else if(currentLine > range.second)
            break;

        string lineNumer = to_string(currentLine) + ") ";
        sf::Text text;
        text.setFont(Resource::instance().getFuncBlockFont());
        text.setString(lineNumer + currentLineString);
        text.setCharacterSize(FB_CHAR_SIZE);
        text.setPosition(0, startYPos);

        if(isDetectedFuncListContainsLine(currentLine))
            text.setFillColor(sf::Color::Red);
        else
            text.setFillColor(sf::Color::Black);

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

bool FunctionBlock::isContainsPoint(sf::Vector2f point)
{
    sf::Vector2f topLeftCornerPos       {getPosition()};
    sf::Vector2f bottomRightCornerPos   {static_cast<float>(getSize().x) + getPosition().x,
                                         static_cast<float>(getSize().y) + getPosition().y};

    if(point.x > topLeftCornerPos.x && point.x < bottomRightCornerPos.x
    && point.y > topLeftCornerPos.y && point.y < bottomRightCornerPos.y)
        return true;
    else
        return false;
}

bool FunctionBlock::isDetectedFuncListContainsLine(unsigned int line)
{
    for(unsigned int functionLine : funtionLineDetectedList)
    {
        if(functionLine == line)
            return true;
    }

    return false;
}

string FunctionBlock::getFuncNameFromPoint(sf::Vector2f point)
{
    for(sf::Text& text: rows)
    {
        sf::Vector2f fbPos = getPosition();
        sf::Vector2f textPos = text.getPosition();
        sf::FloatRect bounds = text.getLocalBounds();
        bounds.left += fbPos.x + FB_PADDING + FB_BORDER_THICKNESS;
        bounds.top += fbPos.y + textPos.y + FB_PADDING + FB_BORDER_THICKNESS;
        bounds.width += fbPos.x + FB_PADDING + FB_BORDER_THICKNESS;
        bounds.height += fbPos.y + textPos.y + FB_PADDING + FB_BORDER_THICKNESS;

        if(point.x > bounds.left && point.x < bounds.width
        && point.y > bounds.top  && point.y < bounds.height)
        {
            return text.getString();
        }
    }

    return "";
}

void FunctionBlock::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(searchingFunction, states);
    target.draw(titlePath, states);

    for(sf::Text text : rows)
        target.draw(text, states);

    target.draw(border, states);
}
