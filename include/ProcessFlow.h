#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <map>
#include <windows.h>
#include <iostream>

#include <FilesTreeElement.h>

using namespace std;

class Process
{
    public:
        Process(int argc, char *argv[]);
        ~Process();

        string getExePath() const { return exePath; }
        string getExeFolderPath() const { return exeFolderPath; }
        string getRelMainFilePath() const { return relativeMainFilePath; }
        string getAbsMainFilePath() const { return absoluteMainFilePath; }

        // stage 1
        bool recursiveFolderSearch(const string& folderPath);

    private:
        string exePath;
        string exeFolderPath;
        string relativeMainFilePath;
        string absoluteMainFilePath;
        string mainFunction;

        map<string, FilesTreeElement> filesTree;

        bool isFileIsHeader(const string& extension);
        bool isFileIsSource(const string& extension);
};

#endif // PROCESS_H
