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
        rowerFileException(void) noexcept = default;
        virtual ~rowerFileException(void) noexcept = default;
        virtual const char* what(void) noexcept = 0;
};

class rowerFileWrongFilePath : public rowerFileException
{
    public:
        rowerFileWrongFilePath(void) noexcept = default;
        virtual ~rowerFileWrongFilePath(void) noexcept = default;
        virtual const char* what(void) noexcept { return "Rowed File : wrong file path"; };
};

class rowerFileHasNotCreated : public rowerFileException
{
    public:
        rowerFileHasNotCreated(void) noexcept = default;
        virtual ~rowerFileHasNotCreated(void) noexcept = default;
        virtual const char* what(void) noexcept { return "Rowed File : rowed file hasn't created"; };
};

struct CharInfo
{
    CharInfo(void) = default;
    CharInfo(char character, uint32_t line, uint32_t position) : c(character), line(line), pos(position) {}

    char c {0};
    uint32_t line {0};
    uint32_t pos {0};
};

class RowedFile
{
    public:
        RowedFile(void) = default;
        RowedFile(const string& fullFilePath);
        ~RowedFile(void) = default;

        bool isEmpty(void) const { return rows.empty(); }
        bool isEOF(void) const { return iterator == rows.end(); }

        string getNextRow(void);
        string getPath(void) const { return path; }
        size_t getSize(void) const { return rows.size(); }

        void resetIteratorPtr(void) { iterator = rows.begin(); }
        void resetStringIndexPtr(void)
        {
            resetIteratorPtr();
            stringIndex = 0;
            currentLine = 1;
        }

        CharInfo get(void);
        CharInfo peek(void);

    private:
        string path {};

        list<string>rows;
        list<string>::iterator iterator {};
        size_t stringIndex {0};
        size_t currentLine {1};
};

#endif // ROWEDFILE_H
