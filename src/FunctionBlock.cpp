#include <FunctionBlock.h>

FunctionBlock::FunctionBlock(ParsedFilePtr parsedFilePtr, const string& functionName)
{
    setOrigin(-(FB_PADDING+FB_BORDER_THICKNESS), -(FB_PADDING+FB_BORDER_THICKNESS));
    uint64_t startYPos   {0};
    uint64_t startXPos   {0};
    uint64_t maxWidth    {0};
    uint64_t currentLine {0};
    sf::FloatRect floatRect;

    absolutePath.setFont(Resource::instance().getFuncBlockFont());
    absolutePath.setString(parsedFilePtr->getAbsoluteFilePath());
    absolutePath.setCharacterSize(FB_CHAR_SIZE + 2);
    absolutePath.setFillColor(sf::Color::Red);
    absolutePath.setPosition(0, startYPos);

    floatRect = absolutePath.getLocalBounds();
    if(floatRect.width > maxWidth)
        maxWidth = floatRect.width;

    startYPos += floatRect.height + FB_GAP_BETWEEN_PATH_AND_NAME;

    name.setFont(Resource::instance().getFuncBlockFont());
    name.setString(functionName);
    name.setCharacterSize(FB_CHAR_SIZE + 4);
    name.setFillColor(sf::Color(0, 128, 64));
    name.setPosition(0, startYPos);

    floatRect = name.getLocalBounds();
    if(floatRect.width > maxWidth)
        maxWidth = floatRect.width;

    startYPos += floatRect.height + FB_GAP_BETWEEN_NAME_AND_CODE;

    RowedFilePtr rowedFilePtr = parsedFilePtr->getRowedFile();
    rowedFilePtr->resetIteratorPtr();

    FunctionInfoPtr mainFunctionInfo = parsedFilePtr->getFunctionInfo(functionName);
    Pos mainFunctionPos = mainFunctionInfo->getLine();
    FunctionInfoListPtr functions = mainFunctionInfo->getFunctionList();

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

        pair<uint64_t, TextExt> row(currentLineAmountString.size(), codeLine);
        rows.push_front(row);
    }

    border.setSize(sf::Vector2f(maxWidth + (2*FB_PADDING), startYPos + (2*FB_PADDING)));
    border.setPosition(-FB_PADDING, -FB_PADDING);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(FB_BORDER_THICKNESS);
    border.setOutlineColor(sf::Color(128,0,0));

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

    for(pair<uint64_t, TextExt> row : rows)
        target.draw(row.second, states);

    target.draw(border, states);
}
