#include "ParsedFile.h"

ParsedFile::ParsedFile(const string& fullFilePath)
{
    absoluteFilePath = fullFilePath;
    fileExtension = fullFilePath.substr(fullFilePath.find_last_of(".") + 1); // without dot
    rowedFilePtr = make_shared<RowedFile>(fullFilePath);
    functionsDefinitionName = make_shared<list<string>>();
}

ParsedFile::~ParsedFile()
{
    //dtor
}

bool ParsedFile::isFileHeader(const string& extension)
{
    if(extension.compare("h") == 0)
        return true;
    else
        return false;
}

bool ParsedFile::isFileSource(const string& extension)
{
    if(extension.compare("c") == 0)
        return true;
    else
        return false;
}

bool ParsedFile::parse()
{
    Lexer lexer(rowedFilePtr);
    TokenList tokenList = lexer.parse();

    for(Token currentToken = tokenList.get(); currentToken.is_not(Token::Kind::End); currentToken = tokenList.get())
    {
        if(currentToken.is(Token::Kind::Unexpected))
        {
            Log << "\t\tUNEXPECTED TOKEN ("<< currentToken.line() << ") : " << currentToken.lexeme() << Logger::endl;
            return false;
        }
        else if(currentToken.is(Token::Kind::Identifier))
        {
            try
            {
                uint64_t peekIndex {0};
                Token openFunctionArgumentBracketToken = tokenList.peek(peekIndex++);
                Token closeFunctionArgumentBracketToken;
                if(openFunctionArgumentBracketToken.is(Token::Kind::LeftParen))
                {
                    uint32_t openBracketCount   {1};
                    uint32_t closeBracketCount  {0};
                    do
                    {
                        closeFunctionArgumentBracketToken = tokenList.peek(peekIndex++);
                        if(closeFunctionArgumentBracketToken.is(Token::Kind::LeftParen))
                            openBracketCount++;
                        else if(closeFunctionArgumentBracketToken.is(Token::Kind::RightParen))
                            closeBracketCount++;
                    }while(openBracketCount != closeBracketCount);

                    Token openCurlyBracketToken = tokenList.peek(peekIndex++);
                    Token closeCurlyOrIdentifierToken;
                    if(openCurlyBracketToken.is(Token::Kind::LeftCurly))
                    {
                        tokenList.seek(peekIndex);
                        peekIndex = 0;
                        openBracketCount = 1;
                        closeBracketCount = 0;
                        FunctionInfoListPtr functions = make_shared<list<shared_ptr<FunctionInfo>>>();

                        do
                        {
                            closeCurlyOrIdentifierToken = tokenList.get();
                            if(closeCurlyOrIdentifierToken.is(Token::Kind::LeftCurly))
                                openBracketCount++;
                            else if(closeCurlyOrIdentifierToken.is(Token::Kind::RightCurly))
                                closeBracketCount++;
                            else
                            {
                                if(closeCurlyOrIdentifierToken.is(Token::Kind::Identifier))
                                {
                                    Token token = tokenList.peek();
                                    if(token.is(Token::Kind::LeftParen))
                                    {
                                        FunctionInfoPtr fInfo = browseFunctionDefinition(functions, tokenList, closeCurlyOrIdentifierToken);
                                        functions->push_back(fInfo);
                                    }
                                }
                            }
                        }while(openBracketCount != closeBracketCount);

                        Log << "\t\tFunction definition (" << currentToken.line() << "; " << closeCurlyOrIdentifierToken.line() << ") : " << currentToken.lexeme() << Logger::endl;

                        functionsDefinitionName->push_back(currentToken.lexeme());
                        FunctionInfoPtr functionDefinitions = make_shared<FunctionInfo>(currentToken.lexeme(),
                                                                                                 Pos(currentToken.line(), closeCurlyOrIdentifierToken.line()),
                                                                                                 Pos(0, 0));
                        functionDefinitions->setFunctionList(functions);
                        functionsDefinition.emplace(currentToken.lexeme(), functionDefinitions);

                        for(FunctionInfoPtr fInfo : *functions)
                            Log << "\t\t\tFunction line(" << fInfo->getLine().first << "; " << fInfo->getLine().second << ")"
                            << " pos(" << fInfo->getPos().first << "; " << fInfo->getPos().second << ") : "
                            << fInfo->getName() << Logger::endl;
                    }
                }
            }
            catch(TokenListEndToken endToken)
            {
                Log << endToken.what() << Logger::endl;
            }
        }
    }

    return true;
}

FunctionInfoPtr ParsedFile::browseFunctionDefinition(FunctionInfoListPtr functionList, TokenList& tokenList, Token currentToken)
{
    tokenList.get();
    uint32_t openBracketCount   {1};
    uint32_t closeBracketCount  {0};
    Token token;

    do
    {
        token = tokenList.get();
        if(token.is(Token::Kind::LeftParen))
            openBracketCount++;
        else if(token.is(Token::Kind::RightParen))
            closeBracketCount++;
        else if(token.is(Token::Kind::Identifier))
        {
            Token nextToken = tokenList.peek();
            if(nextToken.is(Token::Kind::LeftParen))
            {
                FunctionInfoPtr fInfo = browseFunctionDefinition(functionList, tokenList, token);
                functionList->push_back(fInfo);
            }
        }
    }while(openBracketCount != closeBracketCount);

    FunctionInfoPtr functionInfo = make_shared<FunctionInfo> (currentToken.lexeme(),
                                                                       Pos(currentToken.line(), token.line()),
                                                                       Pos(currentToken.pos().first, token.pos().first));

    return functionInfo;
}

FunctionInfoPtr ParsedFile::getFunctionInfo(const string& functionName)
{
    try
    {
        FunctionInfoPtr FunctionInfo = functionsDefinition.at(functionName);
    }
    catch(const std::out_of_range& oor)
    {

    }

    return nullptr;
}
