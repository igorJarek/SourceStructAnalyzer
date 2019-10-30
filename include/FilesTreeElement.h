#ifndef FILESTREEELEMENT_H
#define FILESTREEELEMENT_H

#include <string>

using namespace std;

class FilesTreeElement
{
    public:
        FilesTreeElement();

        void setHeaderPath(const string& header) { headerPath = header; }
        void setSourcePath(const string& source) { sourcePath = source; }

        string getHeaderPath() const { return headerPath; }
        string getSourcePath() const { return sourcePath; }

        bool isHeaderPathSet() const { return headerPath.size() != 0; }
        bool isSourcePathSet() const { return sourcePath.size() != 0; }

    private:
        string headerPath;
        string sourcePath;
};

#endif // FILESTREEELEMENT_H
