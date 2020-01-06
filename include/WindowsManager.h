#ifndef WINDOWSMANAGER_H
#define WINDOWSMANAGER_H

#include <viewController/ViewController.h>
#include <viewController/MainViewController.h>
#include <viewController/StackViewController.h>
#include <ViewController/ConsoleViewController.h>
#include <Typedefs.h>
#include <map>

using namespace std;
using namespace sf;

class WindowsManager
{
    private:
        WindowsManager();
        WindowsManager(const WindowsManager&);
        ~WindowsManager();

    public:
        static WindowsManager& get()
        {
            static WindowsManager instance;
            return instance;
        }

        void addMainViewController(MainViewControllerPtr mainViewControllerPtr);
        void addViewController(ViewControllerPtr viewControllerPtr, const string& name);

        void pool();

    private:
        MainViewControllerPtr m_mainViewControllerPtr;
        map<string, ViewControllerPtr> m_viewControllers;
};

#endif // WINDOWSMANAGER_H
