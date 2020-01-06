#include <WindowsManager.h>
#include <ProcessFlow.h>

#include "version.h"

int main(int argc, char *argv[])
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    sf::VideoMode mainVideoMode(desktop.width, desktop.height);
    MainViewControllerPtr mainViewControllerPtr = make_shared<MainViewController>(mainVideoMode, "Source Structure Analyzer " + VERSION_STRING);
    WindowsManager::get().addMainViewController(mainViewControllerPtr);

    sf::VideoMode stackVideoMode(400, 200);
    ViewControllerPtr stackViewControllerPtr = make_shared<StackViewController>(stackVideoMode, "Queue");
    WindowsManager::get().addViewController(stackViewControllerPtr, "queue");

    ProcessFlow processFlow{argc, argv};
    processFlow.addMainViewController(mainViewControllerPtr);
    processFlow.analyze();

    WindowsManager::get().pool();
    return 0;
}
