#ifndef WINDOWSMANAGER_H
#define WINDOWSMANAGER_H

#include <viewController/ViewController.h>
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

        void addMainViewController(ViewControllerPtr viewControllerPtr);
        void addViewController(ViewControllerPtr viewControllerPtr, const string& name);

        void pool();

    private:
        ViewControllerPtr m_mainViewControllerPtr;
        map<string, ViewControllerPtr> m_viewControllers;
};

#endif // WINDOWSMANAGER_H
