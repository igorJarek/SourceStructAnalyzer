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

class rowerFileHasNotCreated : public rowerFileException
{
    public:
        rowerFileHasNotCreated() noexcept = default;
        virtual ~rowerFileHasNotCreated() noexcept = default;
        virtual const char* what() noexcept { return "Rowed File : rowed file hasn't created"; };
};

struct CharInfo
{
    CharInfo() = default;
    CharInfo(char character, uint32_t line, uint32_t position) : c(character), line(line), pos(position) {}

    char c          {0};
    uint32_t line   {0};
    uint32_t pos    {0};
};

class RowedFile
{
    public:
        RowedFile() = default;
        RowedFile(const string& fullFilePath);
        ~RowedFile() = default;

        bool isEmpty() const { return rows.empty(); }
        bool isEOF() const { return iterator == rows.end(); }

        string getNextRow();
        string getPath() const { return path; }
        size_t getSize() const { return rows.size(); }

        void resetIteratorPtr() { iterator = rows.begin(); }
        void resetStringIndexPtr()
        {
            resetIteratorPtr();
            stringIndex = 0;
            currentLine = 1;
        }

        CharInfo get();
        CharInfo peek();

    private:
        string path {};

        list<string>rows;
        list<string>::iterator iterator {};
        size_t stringIndex {0};
        size_t currentLine {1};
};

#endif // ROWEDFILE_H
