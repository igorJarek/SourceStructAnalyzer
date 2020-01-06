#include "WindowsManager.h"

WindowsManager::WindowsManager()
{

}

WindowsManager::~WindowsManager()
{

}

void WindowsManager::addMainViewController(MainViewControllerPtr mainViewControllerPtr)
{
    m_mainViewControllerPtr = mainViewControllerPtr;
}

void WindowsManager::addViewController(ViewControllerPtr viewControllerPtr, const string& name)
{
    m_viewControllers.insert(pair<string, ViewControllerPtr>(name, viewControllerPtr));
}

void WindowsManager::pool()
{
    RenderWindow& mainWindow = m_mainViewControllerPtr->getWindow();
    while(mainWindow.isOpen())
    {
        sf::Event event;
        while (mainWindow.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                m_mainViewControllerPtr->close(event);

            else if(event.type == sf::Event::Resized)
                m_mainViewControllerPtr->resize(event);

            else if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                    m_mainViewControllerPtr->leftButtonPressed(event);
                else if(event.mouseButton.button == sf::Mouse::Right)
                    m_mainViewControllerPtr->rightButtonPressed(event);
                else if(event.mouseButton.button == sf::Mouse::Middle)
                    m_mainViewControllerPtr->midleButtonPressed(event);
            }
            else if(event.type == sf::Event::MouseButtonReleased)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                    m_mainViewControllerPtr->leftButtonReleased(event);
                else if(event.mouseButton.button == sf::Mouse::Right)
                    m_mainViewControllerPtr->rightButtonReleased(event);
                else if(event.mouseButton.button == sf::Mouse::Middle)
                    m_mainViewControllerPtr->midleButtonReleased(event);
            }
            else if(event.type == sf::Event::MouseMoved)
                m_mainViewControllerPtr->mouseMove(event);
            else if(event.type == sf::Event::MouseWheelScrolled)
                m_mainViewControllerPtr->mouseWheel(event);
        }

        for(pair<string, ViewControllerPtr> viewController : m_viewControllers)
        {
            string windowName = viewController.first;
            ViewControllerPtr viewControllerPtr = viewController.second;
            RenderWindow& renderWindow = viewControllerPtr->getWindow();

            if(renderWindow.isOpen())
            {
                sf::Event childEvent;
                while (renderWindow.pollEvent(childEvent))
                {
                    if(childEvent.type == sf::Event::Closed)
                        viewControllerPtr->close(childEvent);

                    else if(childEvent.type == sf::Event::Resized)
                        viewControllerPtr->resize(childEvent);

                    else if(childEvent.type == sf::Event::MouseButtonPressed)
                    {
                        if(childEvent.mouseButton.button == sf::Mouse::Left)
                            viewControllerPtr->leftButtonPressed(childEvent);
                        else if(childEvent.mouseButton.button == sf::Mouse::Right)
                            viewControllerPtr->rightButtonPressed(childEvent);
                        else if(childEvent.mouseButton.button == sf::Mouse::Middle)
                            viewControllerPtr->midleButtonPressed(childEvent);
                    }
                    else if(childEvent.type == sf::Event::MouseButtonReleased)
                    {
                        if(childEvent.mouseButton.button == sf::Mouse::Left)
                            viewControllerPtr->leftButtonReleased(childEvent);
                        else if(childEvent.mouseButton.button == sf::Mouse::Right)
                            viewControllerPtr->rightButtonReleased(childEvent);
                        else if(childEvent.mouseButton.button == sf::Mouse::Middle)
                            viewControllerPtr->midleButtonReleased(childEvent);
                    }
                    else if(childEvent.type == sf::Event::MouseMoved)
                        viewControllerPtr->mouseMove(childEvent);
                    else if(childEvent.type == sf::Event::MouseWheelScrolled)
                        viewControllerPtr->mouseWheel(childEvent);
                    else if(childEvent.type == sf::Event::KeyPressed)
                        viewControllerPtr->keyboardPressed(childEvent);
                    else if(childEvent.type == sf::Event::KeyReleased)
                        viewControllerPtr->keyboardReleased(childEvent);
                }

                viewControllerPtr->draw();
            }
        }

        m_mainViewControllerPtr->draw();
    }
}
