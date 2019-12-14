#ifndef PARSEDFILE_H
#define PARSEDFILE_H

#include <RowedFile.h>
#include <Lexer.h>
#include <Token.h>
#include <FunctionInfo.h>
#include <TokenList.h>
#include <Logger.h>
#include <Typedefs.h>

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

        bool parse();

        StringListPtr getFunctionsDefinitionName() { return functionsDefinitionName; }
        string getAbsoluteFilePath(void) const { return absoluteFilePath; }
        FunctionInfoPtr getFunctionInfo(const string& functionName);

    private:
        FunctionInfoPtr browseFunctionDefinition(FunctionInfoListPtr functionList, TokenList& tokenList, Token currentToken);

        string absoluteFilePath {};
        string fileExtension {};

        RowedFilePtr rowedFilePtr;
        map<string, FunctionInfoPtr> functionsDefinition;
        StringListPtr functionsDefinitionName;
};

#endif // PARSEDFILE_H
