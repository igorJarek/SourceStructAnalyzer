#ifndef FILESTREEELEMENT_H
#define FILESTREEELEMENT_H

#include <string>
#include <RowedFile.h>

using namespace std;

class FilesTreeElement
{
    public:
        FilesTreeElement();

        void setHeaderPath(const string& header);
        void setSourcePath(const string& source);

        string getHeaderPath() const { return headerPath; }
        string getSourcePath() const { return sourcePath; }
        RowedFile& getHeaderFile() { return headerFile; }
        RowedFile& getSourceFile() { return sourceFile; }

        bool isHeaderPathSet() const { return headerPath.size() != 0; }
        bool isSourcePathSet() const { return sourcePath.size() != 0; }

    private:
        string headerPath {};
        string sourcePath {};
        RowedFile headerFile {};
        RowedFile sourceFile {};
};

#endif // FILESTREEELEMENT_H
