#include "ParsedFile.h"

ParsedFile::ParsedFile(const string& fullFilePath)
{
    rowedFilePtr = make_shared<RowedFile>(fullFilePath);
}

ParsedFile::~ParsedFile()
{
    //dtor
}

bool ParsedFile::parse()
{
    Lexer lexer(rowedFilePtr);
    return true;
}
