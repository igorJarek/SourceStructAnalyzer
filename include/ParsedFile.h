#ifndef PARSEDFILE_H
#define PARSEDFILE_H

#include <RowedFile.h>
#include <Lexer.h>
#include <Token.h>
#include <FunctionInfo.h>
#include <TokenList.h>
#include <Logger.h>

#include <map>
#include <list>
#include <ctype.h>

class ParsedFile
{
    public:
        ParsedFile(const string& fullFilePath);
        ~ParsedFile();

        bool parse();
        shared_ptr<list<string>> getFunctionsDefinitionName() { return functionsDefinitionName; }

        static bool isFileHeader(const string& extension);
        static bool isFileSource(const string& extension);

    private:
        FunctionInfo browseFunctionDefinition(shared_ptr<list<FunctionInfo>> functionList, TokenList& tokenList, Token currentToken);

        string filePath {};
        string fileExtension {};

        shared_ptr<RowedFile> rowedFilePtr;
        map<string, FunctionInfo> functionsDefinition;
        shared_ptr<list<string>> functionsDefinitionName;
};

#endif // PARSEDFILE_H
