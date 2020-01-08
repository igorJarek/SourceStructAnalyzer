#include <FunctionBlock.h>

FunctionBlock::FunctionBlock(ParsedFilePtr parsedFilePtr, const string& functionName)
{
    setOrigin(-(FB_PADDING+FB_BORDER_THICKNESS), -(FB_PADDING+FB_BORDER_THICKNESS));
    uint64_t startYPos   {0};
    uint64_t maxWidth    {0};
    uint64_t currentLine {0};
    sf::FloatRect floatRect;

    absolutePath.setFont(Resource::instance().getFuncBlockFont());
    absolutePath.setString(parsedFilePtr->getAbsoluteFilePath());
    absolutePath.setCharacterSize(FB_CHAR_SIZE + 2);
    absolutePath.setFillColor(FB_PATH_COLOR);
    absolutePath.setPosition(0, startYPos);

    floatRect = absolutePath.getLocalBounds();
    if(floatRect.width > maxWidth)
        maxWidth = floatRect.width;

    startYPos += floatRect.height + FB_GAP_BETWEEN_PATH_AND_NAME;

    name.setFont(Resource::instance().getFuncBlockFont());
    name.setString(functionName);
    name.setCharacterSize(FB_CHAR_SIZE + 6);
    name.setFillColor(FB_NAME_COLOR);
    name.setPosition(0, startYPos);

    floatRect = name.getLocalBounds();
    if(floatRect.width > maxWidth)
        maxWidth = floatRect.width;

    startYPos += floatRect.height + FB_GAP_BETWEEN_NAME_AND_CODE;

    RowedFilePtr rowedFilePtr = parsedFilePtr->getRowedFile();
    rowedFilePtr->resetIteratorPtr();

    FunctionDefinitionPtr mainFunctionDefinition = parsedFilePtr->getFunctionDefinition(functionName);
    Pos mainFunctionPos = mainFunctionDefinition->getLine();
    FunctionCallListPtr functionsCallList = mainFunctionDefinition->getFunctionCallList();

    string currentLineString;
    string currentLineAmountString;
    while(!rowedFilePtr->isEOF())
    {
        currentLine++;
        currentLineString = rowedFilePtr->getNextRow();
        if(currentLine < mainFunctionPos.first)
            continue;
        else if(currentLine > mainFunctionPos.second)
            break;

        currentLineAmountString = to_string(currentLine) + ") ";
        TextExt codeLine(currentLineAmountString + currentLineString, Resource::instance().getFuncBlockFont(), FB_CHAR_SIZE);
        codeLine.setPosition(0, startYPos);
        floatRect = codeLine.getLocalBounds();
        if(floatRect.width > maxWidth)
            maxWidth = floatRect.width;

        startYPos += static_cast<uint64_t>(floatRect.height) + FB_GAP_BETWEEN_CODE_AND_CODE;

        // coloring
        list<FunctionPositionInfo> functionPositionInfoList;
        for(FunctionCallPtr fCall : *functionsCallList)
        {
            uint64_t lineNumberOffset {currentLineAmountString.size()};
            if(fCall->getFunctionNameLine() == currentLine)
            {
                Pos functionNamePos = fCall->getFunctionNamePos();
                codeLine.changeCharactersColor(FB_DETECTED_FUNCTION_COLOR, functionNamePos, lineNumberOffset);
                sf::Vector2f firstLetterPos = codeLine.findCharacterPos(functionNamePos.first + lineNumberOffset);
                sf::Vector2f lastLetterPos  = codeLine.findCharacterPos(functionNamePos.second + lineNumberOffset);
                FunctionPositionInfo functionPositionInfo(fCall->getFunctionName(), firstLetterPos.x, lastLetterPos.x);
                functionPositionInfoList.push_back(functionPositionInfo);
            }

            if(fCall->getOpenParenthesisLine() == currentLine)
                codeLine.changeCharacterColor(FB_DETECTED_FUNCTION_COLOR, fCall->getOpenParenthesisPos().first, lineNumberOffset);

            if(fCall->getCloseParenthesisLine() == currentLine)
                codeLine.changeCharacterColor(FB_DETECTED_FUNCTION_COLOR, fCall->getCloseParenthesisPos().first, lineNumberOffset);
        }

        rows.push_front(pair<list<FunctionPositionInfo>, TextExt>(functionPositionInfoList, codeLine));
    }

    border.setSize(sf::Vector2f(maxWidth + (2*FB_PADDING), startYPos + (2*FB_PADDING)));
    border.setPosition(-FB_PADDING, -FB_PADDING);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(FB_BORDER_THICKNESS);
    border.setOutlineColor(sf::Color(200,0,0));

    size.x = maxWidth   + (2*FB_PADDING) + (2*FB_BORDER_THICKNESS);
    size.y = startYPos  + (2*FB_PADDING) + (2*FB_BORDER_THICKNESS);
}

FunctionBlock::~FunctionBlock()
{
    //dtor
}

void FunctionBlock::setWidth(float width)
{
    sf::Vector2f newSize = border.getSize();
    size.x = newSize.x = width;
    border.setSize(newSize);
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

string FunctionBlock::getFunctionNameFromPoint(sf::Vector2f point)
{
    for(pair<list<FunctionPositionInfo>, TextExt> row : rows)
    {
        list<FunctionPositionInfo> functionPositionInfoList = row.first;
        TextExt textExt = row.second;

        sf::Vector2f fbPos = getPosition();
        sf::Vector2f textPos = textExt.getPosition();
        sf::FloatRect bounds = textExt.getLocalBounds();

        bounds.left += fbPos.x + FB_PADDING + FB_BORDER_THICKNESS;
        bounds.top += fbPos.y + textPos.y + FB_PADDING + FB_BORDER_THICKNESS;
        bounds.width += fbPos.x + FB_PADDING + FB_BORDER_THICKNESS;
        bounds.height += fbPos.y + textPos.y + FB_PADDING + FB_BORDER_THICKNESS;

        if(point.x > bounds.left && point.x < bounds.width
        && point.y > bounds.top  && point.y < bounds.height)
        {
            for(FunctionPositionInfo functionPositionInfo : functionPositionInfoList)
            {
                if(point.x >= bounds.left + functionPositionInfo.m_xStart && point.x <= bounds.left + functionPositionInfo.m_xEnd)
                    return functionPositionInfo.m_functionName;
            }
        }
    }

    return "";
}

void FunctionBlock::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(absolutePath, states);
    target.draw(name, states);

    for(pair<list<FunctionPositionInfo>, TextExt> row : rows)
        target.draw(row.second, states);

    target.draw(border, states);
}
