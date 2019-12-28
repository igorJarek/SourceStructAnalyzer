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
                Log << "\t------------------------------------" << Logger::endl;
                Log << "\tParse file : " << absoluteFilePath << Logger::endl;
                ParsedFilePtr parsedFilePtr = make_shared<ParsedFile>(absoluteFilePath);
                parsedFilePtr->parse();

                ++parsedFileCount;

                Log << "\tAdd file to allFilesTree : " << Logger::endl;
                try
                {
                    ParsedFileListPtr parsedFileListPtr = allFilesTree.at(fileWithExtension);
                    parsedFileListPtr->push_back(parsedFilePtr);
                    Log << "\t\t" << fileWithExtension << " -> (" << parsedFileListPtr->size() << ") -> ";
                    for(ParsedFilePtr pFP : *parsedFileListPtr)
                        Log << pFP->getAbsoluteFilePath() << ", ";
                }
                catch(const std::out_of_range& oor)
                {
                    ParsedFileListPtr parsedFileListPtr = make_shared<list<ParsedFilePtr>>();
                    parsedFileListPtr->push_back(parsedFilePtr);
                    allFilesTree.insert(pair<string, ParsedFileListPtr>(fileWithExtension, parsedFileListPtr));
                    Log << "\t\t" << fileWithExtension << " -> (" << parsedFileListPtr->size() << ") -> ";
                    for(ParsedFilePtr pFP : *parsedFileListPtr)
                        Log << pFP->getAbsoluteFilePath() << ", ";
                }

                Log << Logger::endl;

                StringListPtr functionsNameList = parsedFilePtr->getFunctionsDefinitionList();
                if(ParsedFile::isFileSource(fileExtension))
                {
                    Log << "\tAdd function definitions to ParsedFile Tree : " << Logger::endl;
                    for(string functionName : *functionsNameList)
                    {
                        try
                        {
                            ParsedFileListPtr parsedFileListPtr = parsedFileTree.at(functionName);
                            parsedFileListPtr->push_back(parsedFilePtr);
                            Log << "\t\t" << functionName << " (" << parsedFileListPtr->size() << ") -> ";
                            for(ParsedFilePtr pFP : *parsedFileListPtr)
                                Log << pFP->getAbsoluteFilePath() << ", ";
                        }
                        catch(const std::out_of_range& oor)
                        {
                            ParsedFileListPtr parsedFileListPtr = make_shared<list<ParsedFilePtr>>();
                            parsedFileListPtr->push_back(parsedFilePtr);
                            parsedFileTree.insert(pair<string, ParsedFileListPtr>(functionName, parsedFileListPtr));
                            Log << "\t\t" << functionName << " (" << parsedFileListPtr->size() << ") -> ";
                            for(ParsedFilePtr pFP : *parsedFileListPtr)
                                Log << pFP->getAbsoluteFilePath() << ", ";
                        }

                        Log << Logger::endl;
                    }
                }
            }
        }
    }while(FindNextFile(hFind, &findDataStruct) != 0);

    Log << "\t---Parsed file : " << parsedFileCount << " \\ " << filesCount << " ---" << Logger::endl;

    FindClose(hFind);
    return true;
}

bool ProcessFlow::openMainFile()
{
    Log << "Stage 2 : openMainFile" << Logger::endl;
    ParsedFilePtr mainFunctionPtr;

    try
    {
        ParsedFileListPtr parsedFileListPtr = parsedFileTree.at(mainFunction);
        if(parsedFileListPtr->size() == 0)
        {
            Log << "\tSource code doesn't contain " << mainFunction << " function!" << Logger::endl;
            return false;
        }
        else
        {
            for(ParsedFilePtr parsedFilePtr : *parsedFileListPtr)
            {
                string filePath {parsedFilePtr->getAbsoluteFilePath()};
                size_t pos = filePath.find(relativeMainFilePath);
                if(pos != string::npos)
                {
                    mainFunctionPtr = parsedFilePtr;
                    break;
                }
            }

            if(!mainFunctionPtr)
            {
                Log << "\tProgram can't found " << mainFunction << " function!" << Logger::endl;
                return false;
            }
        }
    }
    catch(const std::out_of_range& oor)
    {
        Log << "\tUnable to open main file!" << Logger::endl;
        return false;
    }

    FunctionDefinitionPtr mainFunctionDefinition = mainFunctionPtr->getFunctionDefinition(mainFunction);
    Pos mainFunctionPos = mainFunctionDefinition->getLine();
    FunctionCallListPtr functions = mainFunctionDefinition->getFunctionCallList();

    Log << "\tProgram found " << mainFunction << "<"<< mainFunctionPos.first << "; " << mainFunctionPos.second << "> function in : " \
    << mainFunctionPtr->getAbsoluteFilePath() << Logger::endl;

    for(FunctionCallPtr fCall : *functions)
    {
        Log << "\t\tFunctions call in main function : " << fCall->getFunctionName();
        set<string>::iterator result = fuctionCallsSet.find(fCall->getFunctionName());
        if(result != fuctionCallsSet.end())
        {
            Log << Logger::endl;
            continue;
        }

        Log << " (Add to function calls queue)" << Logger::endl;
        fuctionCallsSet.insert(fCall->getFunctionName());
        functionCallsQueue.push(CallsQueuePair(fCall->getFunctionName(), mainFunctionPtr));
    }

    FunctionBlock mainFunctionBlock(mainFunctionPtr, mainFunction);

    FunctionBlockListPtr functionBlockListPtr = make_shared<list<FunctionBlock>>();
    functionBlockListPtr->push_back(mainFunctionBlock);
    functionBlockVector.push_back(functionBlockListPtr);

    return true;
}

void ProcessFlow::iteratesCallsQueue()
{
    Log << "Stage 3 : iteratesCallsQueue" << Logger::endl;
    do
    {
        size_t stageSize = functionCallsQueue.size();
        if(stageSize)
        {
            FunctionBlockListPtr functionBlockListPtr = make_shared<list<FunctionBlock>>();
            functionBlockVector.push_back(functionBlockListPtr);
        }

        CallsQueuePair currentCallsQueuePair;
        for(size_t i = 0; i < stageSize; i++)
        {
            currentCallsQueuePair = functionCallsQueue.front();
            functionCallsQueue.pop();

            Log << "\tSearching for function definition : " << currentCallsQueuePair.first << " (" << currentCallsQueuePair.second->getAbsoluteFilePath() << ") ..." << Logger::endl;
            try
            {
                ParsedFileListPtr parsedFileListPtr = parsedFileTree.at(currentCallsQueuePair.first);
                if(parsedFileListPtr->size() == 1)
                {
                    ParsedFilePtr parsedFilePtr = parsedFileListPtr->front();
                    Log << "\t\tFound in the -> " << parsedFilePtr->getAbsoluteFilePath() << Logger::endl;

                    FunctionDefinitionPtr functionDefinitionPtr = parsedFilePtr->getFunctionDefinition(currentCallsQueuePair.first);
                    FunctionCallListPtr functions = functionDefinitionPtr->getFunctionCallList();
                    for(FunctionCallPtr fCall : *functions)
                    {
                        string functionCallName = fCall->getFunctionName();
                        set<string>::iterator result = fuctionCallsSet.find(functionCallName);
                        if(result == fuctionCallsSet.end())
                        {
                            fuctionCallsSet.insert(functionCallName);
                            functionCallsQueue.push(CallsQueuePair(functionCallName, parsedFilePtr));
                        }
                    }

                    FunctionBlock functionBlock(parsedFilePtr, currentCallsQueuePair.first);
                    FunctionBlockListPtr lastFunctionBlockListPtr = functionBlockVector.back();
                    lastFunctionBlockListPtr->push_back(functionBlock);
                }
                else
                {
                    Log << "\t\tMultiple definitions (" << parsedFileListPtr->size() << ")" << Logger::endl;
                }
            }
            catch(const std::out_of_range& oor)
            {
                Log << "\t\tFunction definition didn't find" << Logger::endl;
            }
        }
    }while(!functionCallsQueue.empty());
}

void ProcessFlow::prepareFunctionBlocks()
{
    Log << "Stage 4 : prepareFunctionBlocks" << Logger::endl;

    // get stages info (max height stage, stages width and height)
    uint64_t maxHeightStage {0};
    sf::Vector2u stagesInfo[functionBlockVector.size()];

    for(size_t stageListIndex {0}; stageListIndex < functionBlockVector.size(); ++stageListIndex)
    {
        FunctionBlockListPtr fbList = functionBlockVector[stageListIndex];
        for(FunctionBlock& fb : *fbList)
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
    for(size_t stageListIndex {0}; stageListIndex < functionBlockVector.size(); ++stageListIndex)
    {
        FunctionBlockListPtr fbList = functionBlockVector[stageListIndex];

        uint64_t yPosition = (maxHeightStage - stagesInfo[stageListIndex].y) / 2;
        for(FunctionBlock& fb : *fbList)
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
    for(FunctionBlockListPtr fbListPtr : functionBlockVector)
    {
        for(FunctionBlock& fb : *fbListPtr)
        {
            window.draw(fb);
        }
    }
}

void ProcessFlow::lootAtMainFunctionalBlock(sf::RenderWindow& window)
{
    if(functionBlockVector.size() > 0)
    {
        FunctionBlockListPtr firstStage = functionBlockVector[0];
        if(firstStage->size() == 1)
        {
            FunctionBlock& mainFuntion = firstStage->front();

            sf::Vector2f fbPosition = mainFuntion.getPosition();
            sf::Vector2u fbSize = mainFuntion.getSize();
            sf::Vector2u windowSize = window.getSize();

            sf::View currentView{window.getView()};
            currentView.setCenter((windowSize.x - fbSize.x) / 2, fbPosition.y + 30);
            window.setView(currentView);
        }
    }
}
