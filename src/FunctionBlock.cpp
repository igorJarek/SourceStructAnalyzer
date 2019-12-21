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
    absolutePath.setFillColor(sf::Color(80, 80, 255));
    absolutePath.setPosition(0, startYPos);

    floatRect = absolutePath.getLocalBounds();
    if(floatRect.width > maxWidth)
        maxWidth = floatRect.width;

    startYPos += floatRect.height + FB_GAP_BETWEEN_PATH_AND_NAME;

    name.setFont(Resource::instance().getFuncBlockFont());
    name.setString(functionName);
    name.setCharacterSize(FB_CHAR_SIZE + 6);
    name.setFillColor(sf::Color(0, 128, 64));
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
        for(FunctionCallPtr fCall : *functionsCallList)
        {
            uint64_t lineNumberOffset {currentLineAmountString.size()};
            if(fCall->getFunctionNameLine() == currentLine)
                codeLine.changeCharactersColor(FB_DETECTED_FUNCTION_COLOR, fCall->getFunctionNamePos(), lineNumberOffset);

            if(fCall->getOpenParenthesisLine() == currentLine)
                codeLine.changeCharacterColor(FB_DETECTED_FUNCTION_COLOR, fCall->getOpenParenthesisPos().first, lineNumberOffset);

            if(fCall->getCloseParenthesisLine() == currentLine)
                codeLine.changeCharacterColor(FB_DETECTED_FUNCTION_COLOR, fCall->getCloseParenthesisPos().first, lineNumberOffset);
        }

        rows.push_front(codeLine);
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

void FunctionBlock::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(absolutePath, states);
    target.draw(name, states);

    for(TextExt textExt : rows)
        target.draw(textExt, states);

    target.draw(border, states);
}
