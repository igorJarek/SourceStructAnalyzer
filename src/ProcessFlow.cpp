#include "ProcessFlow.h"

ProcessFlow::ProcessFlow(int argc, char *argv[])
{
    if(argc < 2 || argc > 3)
        throw ProcessFlowWrongParamCount();

    exePath = string(argv[0]);
    exeFolderPath = exePath.substr(0, exePath.find_last_of("\\") + 1);

    relativeMainFilePath = string(argv[1]);
    absoluteMainFilePath = exeFolderPath + relativeMainFilePath;

    mainFunction = string(argv[2]);

    Log << "Exe path : " << exePath << Logger::endl;
    Log << "Exe folder path : " << exeFolderPath << Logger::endl;
    Log << "Relative main file path : " << relativeMainFilePath << Logger::endl;
    Log << "Absolute main file path : " << absoluteMainFilePath << Logger::endl;
    Log << "Main function : " << mainFunction << Logger::endl;
    Log << Logger::endl;
}

ProcessFlow::~ProcessFlow()
{
    //dtor
}

bool ProcessFlow::recursiveFolderSearch(const string& folderPath)
{
    Log << "Stage 1 : recursiveFolderSearch -> " << folderPath << Logger::endl;
    WIN32_FIND_DATA findDataStruct;
    string startDir {folderPath + "*.*"};
    HANDLE hFind = FindFirstFile(startDir.c_str(), &findDataStruct);
    if(hFind == INVALID_HANDLE_VALUE)
    {
        Log << "\tError : " << "Unable to open directory (Error code : " << GetLastError() << ") : " << folderPath << Logger::endl;
        return false;
    }

    do
    {
        if(findDataStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
           (strcmp(findDataStruct.cFileName, ".") == 0 || strcmp(findDataStruct.cFileName, "..") == 0))
            continue;

        if(findDataStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            string nextDir {folderPath + findDataStruct.cFileName + "\\"};
            bool ret = recursiveFolderSearch(nextDir);
            if(!ret)
            {
                Log << "\tError : " << "Unable to open directory (Error code : " << GetLastError() << ") : " << nextDir << Logger::endl;
                return false;
            }
        }
        else
        {
            string absoluteFilePath {folderPath + findDataStruct.cFileName};
            string fileWithExtension {findDataStruct.cFileName};
            string fileExtension {fileWithExtension.substr(fileWithExtension.find_last_of(".") + 1)};
            ++filesCount;

            if(ParsedFile::isFileHeader(fileExtension) || ParsedFile::isFileSource(fileExtension))
            {
                Log << "\tParse file : " << absoluteFilePath << Logger::endl;
                ParsedFilePtr parsedFilePtr = make_shared<ParsedFile>(absoluteFilePath);
                bool ret = parsedFilePtr->parse();
                if(!ret)
                    Log << "\t\tUNEXPECTED TOKEN IN : " << absoluteFilePath << Logger::endl;

                ++parsedFileCount;
                StringListPtr functionsNameList = parsedFilePtr->getFunctionsDefinitionName();

                Log << "\tAdd function definitions to ParsedFile Tree : " << Logger::endl;
                for(string functionName : *functionsNameList)
                {
                    Log << "\t\t" << functionName << " -> ";
                    try
                    {
                        ParsedFileListPtr parsedFileListPtr = parsedFileTree.at(functionName);
                        parsedFileListPtr->push_back(parsedFilePtr);
                        Log << "(" << parsedFileListPtr->size() << ") -> ";
                        for(ParsedFilePtr pFP : *parsedFileListPtr)
                            Log << pFP->getAbsoluteFilePath() << ", ";
                    }
                    catch(const std::out_of_range& oor)
                    {
                        ParsedFileListPtr parsedFileListPtr = make_shared<list<ParsedFilePtr>>();
                        parsedFileListPtr->push_back(parsedFilePtr);
                        parsedFileTree.insert(pair<string, ParsedFileListPtr>(functionName, parsedFileListPtr));
                        Log << "(" << parsedFileListPtr->size() << ") -> ";
                        for(ParsedFilePtr pFP : *parsedFileListPtr)
                            Log << pFP->getAbsoluteFilePath() << ", ";
                    }

                    Log << Logger::endl;
                }
            }
        }
    }while(FindNextFile(hFind, &findDataStruct) != 0);

    Log << "\t---Parsed file : " << parsedFileCount << " \\ " << filesCount << " ---" << Logger::endl;

    FindClose(hFind);
    return true;
}

/*void ProcessFlow::openMainFile()
{
    Log << "Stage 2 : openMainFile" << Logger::endl;
    RowedFile mainFile(relativeMainFilePath);
    std::list<unsigned int> functionDetectedLines;

    regex includePattern {"(\\t*| *)#include(\\t*| *)(\\<|\\\")([a-zA-Z0-9_/]+)(\\.h|\\.hpp)(\\>|\\\")"};
    regex basicFuncDetectionPattern {"(\\w+) *\\("};
    regex closeBracket(" *\\} *");

    int lineNumber = 0;
    string currentLine {};

    while(!mainFile.isEOF())
    {
        currentLine = mainFile.getNextRow();
        lineNumber++;

        if(lineNumber < mainFunctionPosition)
        {
            smatch result;
            if(regex_search(currentLine, result, includePattern))   // #include
            {
                string fileInclude = result[4];
                size_t fileIncludePos = fileInclude.rfind('/');
                if(fileIncludePos != string::npos)
                    fileInclude = fileInclude.substr(fileIncludePos + 1, string::npos);

                Log << "\tInclude detected : " << fileInclude << Logger::endl;
                includeList.push_back(fileInclude);
                continue;
            }
        }
        else if(lineNumber == mainFunctionPosition)
            Log << "\tMain function (" << lineNumber << ") : " << currentLine << Logger::endl;
        else if (lineNumber > mainFunctionPosition)
        {
            Log << "\tLine : " << lineNumber << Logger::endl;

            if(regex_match(currentLine, closeBracket)) // function end {
                break;

            smatch result;
            string::const_iterator searchStart(currentLine.cbegin());
            while(regex_search(searchStart, currentLine.cend(), result, basicFuncDetectionPattern)) // functions call
            {
                string functionName = result[1];
                string functionParams = result.suffix();

                if(isFunctionName(functionName) && isFunctionParams(functionParams))
                {
                    map<string, int>::iterator iterator = fuctionCallsMap.find(functionName);
                    if (iterator == fuctionCallsMap.end())
                    {
                        Log << "\t\tFunction detected : " << functionName << Logger::endl;
                        fuctionCallsMap.emplace(functionName, lineNumber);
                        functionCallsQueue.push(functionName);
                        functionDetectedLines.push_back(lineNumber);
                    }
                }

                searchStart = result.suffix().first;
            }
        }
    }

    list<FunctionBlock> stage0;
    mainFile.resetFileReadedPtr();
    FunctionBlock fb {mainFile, "main function", std::pair<int, int>{0, mainFile.getSize()}, functionDetectedLines};
    stage0.push_back(fb);
    stages.push_back(stage0);
}

void ProcessFlow::iteratesCallsQueue()
{
    Log << "Stage 3 : iteratesCallsQueue" << Logger::endl;
    do
    {
        size_t stageSize = functionCallsQueue.size();
        if(stageSize)
        {
            list<FunctionBlock> stage;
            stages.push_back(stage);
        }

        string currentFunctionName {};
        for(size_t i = 0; i < stageSize; i++)
        {
            currentFunctionName = functionCallsQueue.front();
            functionCallsQueue.pop();

            Log << "\tSearching for function definition : " << currentFunctionName << " .... ";

            // dummy method
            for(map<string, FilesTreeElement>::iterator it = filesTree.begin(); it != filesTree.end(); ++it)
            {
                FilesTreeElement fte = it->second;
                if(fte.isSourcePathSet())
                {
                    const string sourcePath = fte.getSourcePath();
                    RowedFile rowedFile = fte.getSourceFile();
                    pair<int, int> functionPosition = rowedFile.getFunctionPosition(currentFunctionName);
                    if(functionPosition.first && functionPosition.second)
                    {
                        Log << " found in the -> " << sourcePath << Logger::endl;
                        Log << "\t\tSearching for function calls .... " << Logger::endl;
                        Log << "\t\t\tRange line : <" << functionPosition.first << "; " << functionPosition.second << ">" << Logger::endl;

                        regex basicFuncDetectionPattern {"(\\w+) *\\("};

                        rowedFile.resetFileReadedPtr();
                        rowedFile.moveFileReaderPtr(functionPosition.first + 1);

                        std::list<unsigned int> functionDetectedLines;
                        string currentLine {};
                        smatch result;
                        for(int foundedFileIndex = functionPosition.first + 1; foundedFileIndex < functionPosition.second; foundedFileIndex++)
                        {
                            currentLine = rowedFile.getNextRow();
                            string::const_iterator searchStart(currentLine.cbegin());
                            while(regex_search(searchStart, currentLine.cend(), result, basicFuncDetectionPattern)) // functions call
                            {
                                string functionName = result[1];
                                string functionParams = result.suffix();

                                if(isFunctionName(functionName) && isFunctionParams(functionParams))
                                {
                                    map<string, int>::iterator iterator = fuctionCallsMap.find(functionName);
                                    if (iterator == fuctionCallsMap.end())
                                    {
                                        Log << "\t\t\tFunction detected (line -> " << foundedFileIndex << ") : " << functionName << Logger::endl;
                                        fuctionCallsMap.emplace(functionName, foundedFileIndex);
                                        functionCallsQueue.push(functionName);
                                    }

                                    functionDetectedLines.push_back(foundedFileIndex);
                                }

                                searchStart = result.suffix().first;
                            }
                        }

                        rowedFile.resetFileReadedPtr();
                        FunctionBlock functionalBlock(rowedFile, currentFunctionName, functionPosition, functionDetectedLines);
                        list<FunctionBlock>& currentStage = stages.back();
                        currentStage.push_back(functionalBlock);

                        break;
                    }
                }
            }
        }
    }while(!functionCallsQueue.empty());
}

void ProcessFlow::prepareFunctionBlocks()
{
    Log << "Stage 4 : prepareFunctionBlocks" << Logger::endl;

    // get stages info (max height stage, stages width and height)
    uint64_t maxHeightStage {0};
    sf::Vector2u stagesInfo[stages.size()];

    for(size_t stageListIndex {0}; stageListIndex < stages.size(); ++stageListIndex)
    {
        list<FunctionBlock>& fbList = stages[stageListIndex];
        for(FunctionBlock& fb : fbList)
        {
            sf::Vector2u fbSize = fb.getSize();
            stagesInfo[stageListIndex].y += fbSize.y + ST_Y_GAP;
            if(fbSize.x > stagesInfo[stageListIndex].x)
                stagesInfo[stageListIndex].x = fbSize.x;
        }

        stagesInfo[stageListIndex].y -= ST_Y_GAP;

        if(stagesInfo[stageListIndex].y > maxHeightStage)
            maxHeightStage = stagesInfo[stageListIndex].y;
    }

    // set initial positions
    Log << "\tSet Initial Positions" << Logger::endl;
    uint64_t xPosition {0};
    for(size_t stageListIndex {0}; stageListIndex < stages.size(); ++stageListIndex)
    {
        list<FunctionBlock>& fbList = stages[stageListIndex];

        uint64_t yPosition = (maxHeightStage - stagesInfo[stageListIndex].y) / 2;
        for(FunctionBlock& fb : fbList)
        {
            Log << "\t\tFB Pos x : " << xPosition << " y : " << yPosition << Logger::endl;
            fb.setPosition(xPosition, yPosition);
            yPosition += fb.getSize().y + ST_Y_GAP;
        }

        xPosition += stagesInfo[stageListIndex].x + ST_X_GAP;
    }
}

void ProcessFlow::drawStages(sf::RenderWindow& window)
{
    for(list<FunctionBlock> fbList : stages)
    {
        for(FunctionBlock& fb : fbList)
        {
            window.draw(fb);
        }
    }
}

void ProcessFlow::lootAtMainFunctionalBlock(sf::RenderWindow& window)
{
    if(stages.size() > 0)
    {
        list<FunctionBlock>& firstStage = stages[0];
        if(firstStage.size() == 1)
        {
            FunctionBlock& mainFuntion = firstStage.front();

            sf::Vector2f fbPosition = mainFuntion.getPosition();
            sf::Vector2u fbSize = mainFuntion.getSize();
            sf::Vector2u windowSize = window.getSize();

            sf::View currentView{window.getView()};
            currentView.setCenter((windowSize.x - fbSize.x) / 2, fbPosition.y + 30);
            window.setView(currentView);
        }
    }
}*/
