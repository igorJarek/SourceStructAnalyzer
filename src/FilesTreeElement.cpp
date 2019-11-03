#include "FilesTreeElement.h"

FilesTreeElement::FilesTreeElement()
{
    //ctor
}

void FilesTreeElement::setHeaderPath(const string& header)
{
     headerPath = header;
     headerFile.create(headerPath);
}
void FilesTreeElement::setSourcePath(const string& source)
{
    sourcePath = source;
    sourceFile.create(sourcePath);
}
