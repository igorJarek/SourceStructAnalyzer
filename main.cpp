#include <SFML/Graphics.hpp>
#include <windows.h>
#include <map>
#include <iostream>

#include "version.h"

#include <RowedFile.h>
#include <FunctionBlock.h>
#include <FilesTreeElement.h>

bool recursiveFolderSearch(const string& folderPath, map<string, FilesTreeElement>& fTree);
bool isFileIsHeader(const string& extension);
bool isFileIsSource(const string& extension);

int main(int argc, char *argv[])
{
    sf::VideoMode vMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(vMode.width, vMode.height), "Source Structure Analyzer " + VERSION_STRING);
    window.setFramerateLimit(60);

    sf::View view(sf::FloatRect(0.f, 0.f, vMode.width, vMode.height));
    window.setView(view);

    sf::Vector2u resizeOldWindowSize {window.getSize()};
    sf::Vector2f moveMousePressPoint {};
    sf::Vector2f moveCenterViewPoint {};
    bool leftMouseState = false;

    RowedFile rowedFile("main.tests");
    FunctionBlock functionalBlock(rowedFile);
    functionalBlock.setPosition(10, 10);

    /// building files, associative tree
    ///               key                 ->              data
    ///              string               ->         FilesTreeElement
    ///    nazwa samego pliku np. usb     -> bezwzgledny adres do plików h i cpp (usb.h and usb.c)

    string exePath = string(argv[0]);
    string exeFolderPath = exePath.substr(0, exePath.find_last_of("\\") + 1);

    string relativeFilePath = string(argv[1]);
    string absoluteFilePath = exeFolderPath + relativeFilePath;

    map<string, FilesTreeElement> filesTree;
    bool ret = recursiveFolderSearch(exeFolderPath, filesTree);

    cout << "Size of map : " << filesTree.size() << endl;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::Resized)
            {
                sf::View currentView{window.getView()};
                currentView.setSize(event.size.width, event.size.height);
                const sf::Vector2i currentWindowSize{resizeOldWindowSize.x - event.size.width, resizeOldWindowSize.y - event.size.height};
                currentView.move(-currentWindowSize.x, -currentWindowSize.y);
                resizeOldWindowSize.x = event.size.width;
                resizeOldWindowSize.y = event.size.height;
                window.setView(currentView);
            }

            if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    leftMouseState = true;
                    moveMousePressPoint.x = event.mouseButton.x;
                    moveMousePressPoint.y = event.mouseButton.y;
                    moveCenterViewPoint = window.getView().getCenter();
                }
            }

            if(event.type == sf::Event::MouseButtonReleased)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                    leftMouseState = false;
            }

            if(event.type == sf::Event::MouseMoved)
            {
                if(leftMouseState)
                {
                    sf::View currentView{window.getView()};
                    currentView.setCenter(moveCenterViewPoint.x - (event.mouseMove.x - moveMousePressPoint.x), moveCenterViewPoint.y - (event.mouseMove.y - moveMousePressPoint.y));
                    window.setView(currentView);
                }
            }

            if(event.type == sf::Event::MouseWheelScrolled)
            {
                if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                {
                    float zoom {1.f + ZOOM_STEP};

                    if (event.mouseWheelScroll.delta > 0)
                        zoom = 1.f / zoom;

                    sf::Vector2i pixel{ event.mouseWheelScroll.x, event.mouseWheelScroll.y };
                    const sf::Vector2f beforeCoord{window.mapPixelToCoords(pixel)};
                    sf::View view{window.getView()};
                    view.zoom(zoom);
                    window.setView(view);
                    const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel)};
                    const sf::Vector2f offsetCoords{beforeCoord - afterCoord};
                    view.move(offsetCoords);
                    window.setView(view);
                }
            }

            if(event.type == sf::Event::KeyPressed)
            {
                sf::View currentView = window.getView();
                sf::Vector2f centerPoint = currentView.getCenter();

                if(event.key.code == sf::Keyboard::Up)
                    currentView.setCenter(centerPoint.x, centerPoint.y - 10.f);
                else if(event.key.code == sf::Keyboard::Right)
                    currentView.setCenter(centerPoint.x - 10.f, centerPoint.y);
                else if(event.key.code == sf::Keyboard::Down)
                    currentView.setCenter(centerPoint.x, centerPoint.y+10.f);
                else if(event.key.code == sf::Keyboard::Left)
                    currentView.setCenter(centerPoint.x + 10.f, centerPoint.y);

                window.setView(currentView);
            }
        }

        window.clear(sf::Color::White);
        window.draw(functionalBlock);
        window.display();
    }

    return 0;
}

bool recursiveFolderSearch(const string& folderPath, map<string, FilesTreeElement>& fTree)
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
                bool ret = recursiveFolderSearch(nextDir, fTree);
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
                    FilesTreeElement& element = fTree.at(fileWithoutExtension);
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

                    fTree.insert(pair<string, FilesTreeElement>(fileWithoutExtension, element));
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

bool isFileIsHeader(const string& extension)
{
    if(extension.compare("h") == 0 || extension.compare("hpp"))
        return true;
    else
        return false;
}

bool isFileIsSource(const string& extension)
{
    if(extension.compare("c") == 0 || extension.compare("cpp"))
        return true;
    else
        return false;
}
