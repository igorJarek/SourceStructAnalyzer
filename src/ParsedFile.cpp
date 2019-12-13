#include "ParsedFile.h"

ParsedFile::ParsedFile(const string& fullFilePath)
{
    filePath = fullFilePath;
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
    Lexer lexer(*rowedFilePtr);
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
                        shared_ptr<list<FunctionInfo>> functions = make_shared<list<FunctionInfo>>();

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
                                        FunctionInfo fInfo = browseFunctionDefinition(functions, tokenList, closeCurlyOrIdentifierToken);
                                        functions->push_back(fInfo);
                                    }
                                }
                            }
                        }while(openBracketCount != closeBracketCount);

                        Log << "\t\tFunction definition (" << currentToken.line() << "; " << closeCurlyOrIdentifierToken.line() << ") : " << currentToken.lexeme() << Logger::endl;

                        functionsDefinitionName->push_back(currentToken.lexeme());
                        FunctionInfo functionDefinitions(currentToken.lexeme(),
                                                         FunctionInfo::Pos(currentToken.line(), closeCurlyOrIdentifierToken.line()),
                                                         FunctionInfo::Pos(0, 0));
                        functionDefinitions.setFunctionList(functions);
                        functionsDefinition.emplace(currentToken.lexeme(), functionDefinitions);

                        for(FunctionInfo fInfo : *functions)
                            Log << "\t\t\tFunction line(" << fInfo.getLine().first << "; " << fInfo.getLine().second << ")"
                            << " pos(" << fInfo.getPos().first << "; " << fInfo.getPos().second << ") : "
                            << fInfo.getName() << Logger::endl;
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

FunctionInfo ParsedFile::browseFunctionDefinition(shared_ptr<list<FunctionInfo>> functionList, TokenList& tokenList, Token currentToken)
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
                FunctionInfo fInfo = browseFunctionDefinition(functionList, tokenList, token);
                functionList->push_back(fInfo);
            }
        }
    }while(openBracketCount != closeBracketCount);

    return FunctionInfo(currentToken.lexeme(),
                        FunctionInfo::Pos(currentToken.line(),      token.line()),
                        FunctionInfo::Pos(currentToken.pos().first, token.pos().first));
}
