#include <WindowsManager.h>
#include <ProcessFlow.h>

#include "version.h"

int main(int argc, char *argv[])
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    MainViewControllerPtr mainViewControllerPtr = make_shared<MainViewController>(desktop, "Source Structure Analyzer " + VERSION_STRING);
    mainViewControllerPtr->setIcon("icons/Main.png");
    WindowsManager::get().addMainViewController(mainViewControllerPtr);

    sf::VideoMode consoleVideoMode(desktop.width, 400);
    ViewControllerPtr consoleViewControllerPtr = make_shared<ConsoleViewController>(consoleVideoMode, "Console", sf::Style::Titlebar | sf::Style::Resize);
    consoleViewControllerPtr->setIcon("icons/Console.png");
    WindowsManager::get().addViewController(consoleViewControllerPtr, "console");


    sf::VideoMode stackVideoMode(400, 200);
    ViewControllerPtr stackViewControllerPtr = make_shared<StackViewController>(stackVideoMode, "Queue", sf::Style::Titlebar | sf::Style::Resize);
    stackViewControllerPtr->setIcon("icons/Queue.png");
    WindowsManager::get().addViewController(stackViewControllerPtr, "queue");

    ProcessFlow processFlow{argc, argv};
    processFlow.addMainViewController(mainViewControllerPtr);
    processFlow.analyze();

    WindowsManager::get().pool();
    return 0;
}
