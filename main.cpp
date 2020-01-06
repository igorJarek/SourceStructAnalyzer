#include <WindowsManager.h>
#include <ProcessFlow.h>

#include "version.h"

int main(int argc, char *argv[])
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::VideoMode videoMode(desktop.width, desktop.height);

    MainViewControllerPtr mainViewControllerPtr = make_shared<MainViewController>(videoMode, "Source Structure Analyzer " + VERSION_STRING);
    WindowsManager::get().addMainViewController(mainViewControllerPtr);

    ProcessFlow processFlow{argc, argv};
    processFlow.addMainViewController(mainViewControllerPtr);
    processFlow.recursiveFolderSearch(processFlow.getExeFolderPath());
    processFlow.openMainFile();
    processFlow.iteratesCallsQueue();

    WindowsManager::get().pool();

    return 0;
}
