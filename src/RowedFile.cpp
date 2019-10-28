#include "RowedFile.h"

RowedFile::RowedFile(const string& fullFilePath)
{
    fstream pFile;
    pFile.open(fullFilePath, fstream::in);
    if(!pFile.is_open())
        throw rowerFileWrongFilePath();

    while(!pFile.eof())
    {
        std::string row;
        getline(pFile, row);
        rows.push_back(row);
    }

    iterator = rows.begin();
    pFile.close();
}

RowedFile::~RowedFile()
{
    //dtor
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
