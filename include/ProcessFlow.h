#ifndef PROCESS_H
#define PROCESS_H

#include <SFML/System/Vector2.hpp>
#include <string>
#include <map>
#include <windows.h>
#include <iostream>
#include <exception>
#include <queue>
#include <vector>
#include <list>
#include <regex>

#include <ParsedFile.h>
#include <FunctionInfo.h>
#include <RowedFile.h>
#include <FunctionBlock.h>
#include <Logger.h>

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

        void goToDefinition(sf::Vector2f clickPoint);

        // stage 1
        bool recursiveFolderSearch(const string& folderPath);
        // stage 2
        void openMainFile();
        // stage 3
        void iteratesCallsQueue();
        // stage 4
        void prepareFunctionBlocks();
        // stage 5
        void drawStages(sf::RenderWindow& window);
        // stage 6
        void lootAtMainFunctionalBlock(sf::RenderWindow& window);

    private:
        using ParsedFilePtr = shared_ptr<ParsedFile>;
        using ParsedFileListPtr = shared_ptr<list<ParsedFilePtr>>;
        using ParsedFilesMap = map<string, ParsedFileListPtr>;

        string exePath;
        string exeFolderPath;
        string relativeMainFilePath;
        string absoluteMainFilePath;
        int mainFunctionPosition;

        ParsedFilesMap parsedFileTree;

        queue<string> functionCallsQueue;
        map<string, int> fuctionCallsMap;
        vector<list<FunctionBlock>>stages;
};

#endif // PROCESS_H
