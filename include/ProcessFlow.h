#ifndef PROCESS_H
#define PROCESS_H

#include <viewController/MainViewController.h>
#include <windows.h>
#include <exception>
#include <map>
#include <queue>
#include <vector>

#include <ParsedFile.h>
#include <FunctionDefinition.h>
#include <RowedFile.h>
#include <FunctionBlock.h>
#include <Logger.h>
#include <Typedefs.h>

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

        string getExePath()         const { return exePath; }
        string getExeFolderPath()   const { return exeFolderPath; }
        string getRelMainFilePath() const { return relativeMainFilePath; }
        string getAbsMainFilePath() const { return absoluteMainFilePath; }

        void addMainViewController(MainViewControllerPtr mainViewControllerPtr);

        void analyze();

    private:
        // stage 1
        bool recursiveFolderSearch(const string& folderPath);
        // stage 2
        bool openMainFile();
        // stage 3
        void iteratesCallsQueue();

        void notifyMainViewController(vector<FunctionBlockListPtr>& functionBlockVector);
        MainViewControllerPtr m_mainViewControllerPtr;

        string exePath;
        string exeFolderPath;
        string relativeMainFilePath;
        string absoluteMainFilePath;
        string mainFunction;

        uint32_t parsedFileCount {0};
        uint32_t filesCount {0};

        map<string, ParsedFileListPtr> parsedFileTree;
        map<string, ParsedFileListPtr> allFilesTree;

        queue<CallsQueuePair> functionCallsQueue;
        set<string> fuctionCallsSet;

        vector<FunctionBlockListPtr> functionBlockVector;
};

#endif // PROCESS_H
