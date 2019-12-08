#ifndef PARSEDFILE_H
#define PARSEDFILE_H

#include <FunctionInfo.h>
#include <RowedFile.h>
#include <Lexer.h>
#include <Token.h>

#include <list>
#include <ctype.h>

class ParsedFile
{
    public:
        ParsedFile(const string& fullFilePath);
        ~ParsedFile();

        bool parse();

    private:

        shared_ptr<RowedFile> rowedFilePtr;
        shared_ptr<list<Token>> tokensList;
};

#endif // PARSEDFILE_H
