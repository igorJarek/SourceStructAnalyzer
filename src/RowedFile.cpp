#include <RowedFile.h>

RowedFile::RowedFile(const string& fullFilePath)
{
    create(fullFilePath);
}

RowedFile::~RowedFile()
{
    //dtor
}

void RowedFile::create(const string& fullFilePath)
{
    if(!loaded)
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
            rows.push_back(row);
        }

        resetFileReadedPtr();
        pFile.close();
        loaded = true;
    }
}

string RowedFile::getNextRow()
{
    string ret;
    if(loaded && iterator != rows.end())
    {
       ret = *iterator;
       iterator++;
    }

    return ret;
}

void RowedFile::moveFileReaderPtr(size_t offset)
{
    if(loaded)
    {
        for(size_t i = 0; i < offset; i++)
            iterator++;
    }
}

pair<int, int> RowedFile::getFunctionPosition(const string& functionName)
{
    if(!loaded)
        throw rowerFileHasNotCreated();

    regex declarationPattern(" *\\w+ *" + functionName + "{1}");
    regex openBracket(" *\\{ *");
    regex closeBracket(" *\\} *");

    pair<int, int> result{0, 0};

    bool stage[3] {false, false, false};
    int currentLine = 0;
    resetFileReadedPtr();

    while(!isEOF())
    {
        currentLine++;

        string input = getNextRow();
        if(!stage[0] && regex_search(input, declarationPattern))
        {
            if(input.rfind(';') == string::npos)
            {
                result.first = currentLine;
                stage[0] = true;
            }

            continue;
        }

        if(stage[0] && !stage[1] && regex_match(input, openBracket))
        {
            stage[1] = true;
            continue;
        }

        if(stage[1] && !stage[2] && regex_match(input, closeBracket))
        {
            result.second = currentLine;
            break;
        }
    }

    return result;
}
