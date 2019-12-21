#ifndef PARSEDFILE_H
#define PARSEDFILE_H

#include <RowedFile.h>
#include <Lexer.h>
#include <Token.h>
#include <FunctionDefinition.h>
#include <TokenList.h>
#include <Logger.h>
#include <Typedefs.h>
#include <FunctionCall.h>

#include <map>
#include <list>
#include <ctype.h>

class ParsedFile
{
    public:
        static bool isFileHeader(const string& extension);
        static bool isFileSource(const string& extension);

    public:
        ParsedFile(const string& fullFilePath);
        ~ParsedFile();

        void parse();

        StringListPtr getFunctionsDefinitionName() { return functionsDefinitionName; }
        string getAbsoluteFilePath(void) const { return absoluteFilePath; }
        FunctionDefinitionPtr getFunctionDefinition(const string& functionName);
        RowedFilePtr getRowedFile(void) { return rowedFilePtr; }

    private:
        FunctionCallPtr findFunctionCalls(FunctionCallListPtr functionCallList, TokenList& tokenList, Token currentToken);
        void removeUnnecessaryTokens(TokenList& tokenList);

        string absoluteFilePath {};
        string fileExtension {};

        RowedFilePtr rowedFilePtr;
        map<string, FunctionDefinitionPtr> functionsDefinition;
        StringListPtr functionsDefinitionName;
};

#endif // PARSEDFILE_H
