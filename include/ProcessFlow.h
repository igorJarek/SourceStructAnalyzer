#ifndef PROCESS_H
#define PROCESS_H

#include <SFML/System/Vector2.hpp>
#include <string>
#include <windows.h>
#include <exception>
#include <map>
#include <queue>
#include <vector>
#include <list>

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

        // stage 1
        bool recursiveFolderSearch(const string& folderPath);
        // stage 2
        bool openMainFile();
        // stage 3
        void iteratesCallsQueue();
        // stage 4
        void prepareFunctionBlocks();
        // stage 5
        void drawStages(sf::RenderWindow& window);
        // stage 6
        void lootAtMainFunctionalBlock(sf::RenderWindow& window);

    private:
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
