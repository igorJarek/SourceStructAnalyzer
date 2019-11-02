#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <map>
#include <windows.h>
#include <iostream>
#include <exception>
#include <queue>

#include <FilesTreeElement.h>
#include <RowedFile.h>

using namespace std;

class ProcessFlowException
{
    public:
        ProcessFlowException() noexcept = default;
        virtual ~ProcessFlowException() noexcept = default;
        virtual const char* what() noexcept = 0;
};

class ProcessFlowWrongParamCount : public ProcessFlowException
{
    public:
        ProcessFlowWrongParamCount() noexcept = default;
        virtual ~ProcessFlowWrongParamCount() noexcept = default;
        virtual const char* what() noexcept { return "ProcessFlow : Program needs 2 params"; };
};

class ProcessFlow
{
    public:
        ProcessFlow(int argc, char *argv[]);
        ~ProcessFlow();

        string getExePath() const { return exePath; }
        string getExeFolderPath() const { return exeFolderPath; }
        string getRelMainFilePath() const { return relativeMainFilePath; }
        string getAbsMainFilePath() const { return absoluteMainFilePath; }

        // stage 1
        bool recursiveFolderSearch(const string& folderPath);
        // stage 2
        void openMainFile();

    private:
        string exePath;
        string exeFolderPath;
        string relativeMainFilePath;
        string absoluteMainFilePath;
        int mainFunctionPosition;

        map<string, FilesTreeElement> filesTree;
        queue<string> functionCallsQueue;

        bool isFileIsHeader(const string& extension);
        bool isFileIsSource(const string& extension);

        bool isFunctionName(const string& functionName);
        bool isFunctionParams(const string& functionParams);
};

#endif // PROCESS_H
