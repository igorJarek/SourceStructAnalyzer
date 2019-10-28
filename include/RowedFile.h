#ifndef ROWEDFILE_H
#define ROWEDFILE_H

#include <fstream>
#include <list>
#include <string>
#include <exception>

using namespace std;

class rowerFileException
{
    public:
        rowerFileException() noexcept = default;
        virtual ~rowerFileException() noexcept = default;
        virtual const char* what() noexcept = 0;
};

class rowerFileWrongFilePath : public rowerFileException
{
    public:
        rowerFileWrongFilePath() noexcept = default;
        virtual ~rowerFileWrongFilePath() noexcept = default;
        virtual const char* what() noexcept { return "Rowed File : wrong file path"; };
};

class RowedFile
{
    public:
        RowedFile(const string& fullFilePath);
        ~RowedFile();

        bool isEmpty() const { return rows.empty(); }
        bool isEOF() const { return iterator == rows.end(); }
        string getNextRow();

    private:
        list<string>rows;
        list<string>::iterator iterator;
};

#endif // ROWEDFILE_H
