#include <RowedFile.h>

RowedFile::RowedFile(const string& fullFilePath)
{
    path = fullFilePath;

    fstream pFile;
    pFile.open(fullFilePath, fstream::in);
    if(!pFile.is_open())
        throw rowerFileWrongFilePath();

    while(!pFile.eof())
    {
        std::string row;
        getline(pFile, row);
        row += '\n';
        rows.push_back(row);
    }

    pFile.close();

    rows.back() += '\0';

    resetIteratorPtr();
    resetStringIndexPtr();
}

CharInfo RowedFile::get()
{
    string s = *iterator;
    char c = s[stringIndex];
    CharInfo cInfo(c, currentLine, stringIndex);

    if(stringIndex + 1 < s.size())
        ++stringIndex;
    else
    {
        ++iterator;
        if(iterator != rows.end())
        {
            stringIndex = 0;
            ++currentLine;
        }
    }

    return cInfo;
}

CharInfo RowedFile::peek()
{
    string s = *iterator;
    char c = s[stringIndex];
    CharInfo cInfo(c, currentLine, stringIndex);

    return cInfo;
}

string RowedFile::getNextRow()
{
    string ret;
    if(iterator != rows.end())
    {
       ret = *iterator;
       iterator++;
    }

    return ret;
}
