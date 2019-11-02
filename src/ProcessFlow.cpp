#include "ProcessFlow.h"

ProcessFlow::ProcessFlow(int argc, char *argv[])
{
    if(argc < 2 || argc > 3)
        throw ProcessFlowWrongParamCount();

    exePath = string(argv[0]);
    exeFolderPath = exePath.substr(0, exePath.find_last_of("\\") + 1);

    relativeMainFilePath = string(argv[1]);
    absoluteMainFilePath = exeFolderPath + relativeMainFilePath;

    mainFunctionPosition = stoi(argv[2]);
}

ProcessFlow::~ProcessFlow()
{
    //dtor
}

bool ProcessFlow::isFileIsHeader(const string& extension)
{
    if(extension.compare("h") == 0 || extension.compare("hpp"))
        return true;
    else
        return false;
}

bool ProcessFlow::isFileIsSource(const string& extension)
{
    if(extension.compare("c") == 0 || extension.compare("cpp"))
        return true;
    else
        return false;
}

bool ProcessFlow::recursiveFolderSearch(const string& folderPath)
{
    WIN32_FIND_DATA findDataStruct;
    string startDir = folderPath + "*.*";
    HANDLE hFind = FindFirstFile(startDir.c_str(), &findDataStruct);
    if(hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(findDataStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
               (strcmp(findDataStruct.cFileName, ".") == 0 || strcmp(findDataStruct.cFileName, "..") == 0))
                continue;

            if(findDataStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                string nextDir = folderPath + findDataStruct.cFileName + "\\";
                bool ret = recursiveFolderSearch(nextDir);
                if(!ret)
                {
                    cout << "Unable to open directory (Error " << GetLastError() << ") : " << nextDir << endl;
                    return false;
                }
            }
            else
            {
                string absoluteFilePath = folderPath + findDataStruct.cFileName;
                string fileWithExtension = findDataStruct.cFileName;
                string fileWithoutExtension = fileWithExtension.substr(0, fileWithExtension.find_last_of("."));
                string fileExtension = fileWithExtension.substr(fileWithExtension.find_last_of(".") + 1);

                try
                {
                    FilesTreeElement& element = filesTree.at(fileWithoutExtension);
                    if(element.isHeaderPathSet() && isFileIsSource(fileExtension))
                        element.setSourcePath(absoluteFilePath);
                    else if(element.isSourcePathSet() && isFileIsHeader(fileExtension))
                        element.setHeaderPath(absoluteFilePath);
                }
                catch(const std::out_of_range& oor)
                {
                    FilesTreeElement element;

                    if(isFileIsHeader(fileExtension))
                        element.setHeaderPath(absoluteFilePath);
                    else if(isFileIsSource(fileExtension))
                        element.setSourcePath(absoluteFilePath);

                    filesTree.insert(pair<string, FilesTreeElement>(fileWithoutExtension, element));
                }
            }
        }while(FindNextFile(hFind, &findDataStruct) != 0);
    }
    else
    {
        cout << "Unable to open directory (Error " << GetLastError() << ") : " << folderPath << endl;
        return false;
    }

    FindClose(hFind);
    return true;
}

bool ProcessFlow::isFunctionName(const string& functionName)
{
    if(functionName.compare("if") == 0)
        return false;
    else if(functionName.compare("while") == 0)
        return false;
    else if(functionName.compare("switch") == 0)
        return false;
    else if(functionName.compare("sizeof") == 0)
        return false;

    return true;
}

bool ProcessFlow::isFunctionParams(const string& functionParams)
{
    // TODO
    return true;
}

void ProcessFlow::openMainFile()
{
    RowedFile mainFile(relativeMainFilePath);
    regex basicFuncDetectionPattern {"(\\w+) *\\("};
    regex closeBracket(" *\\} *");

    int lineNumber = 0;
    string currentLine {};

    while(!mainFile.isEOF())
    {
        currentLine = mainFile.getNextRow();
        lineNumber++;

        if(lineNumber < mainFunctionPosition)
            continue;
        else if(lineNumber == mainFunctionPosition)
            cout << "Main function (" << lineNumber << ") : " << currentLine << endl;
        else if (lineNumber > mainFunctionPosition)
        {
            if(regex_match(currentLine, closeBracket))
                break;

            cout << "Line : " << lineNumber << endl;
            smatch result;
            string::const_iterator searchStart(currentLine.cbegin());
            while(regex_search(searchStart, currentLine.cend(), result, basicFuncDetectionPattern))
            {
                string functionName = result[1];
                string functionParams = result.suffix();

                if(isFunctionName(functionName) && isFunctionParams(functionParams))
                {
                    cout << "\tFunction detected : " << functionName << endl;
                    functionCallsQueue.push(functionName);
                }

                searchStart = result.suffix().first;
            }
        }
    }
}
