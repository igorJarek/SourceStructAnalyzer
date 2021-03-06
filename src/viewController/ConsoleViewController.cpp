#include "ConsoleViewController.h"

ConsoleViewController::ConsoleViewController(VideoMode& mode, const string& title, Uint32 style) : ViewController(mode, title, style)
{
    //m_renderWindow.setFramerateLimit(CONSOLE_VIEW_FRAMERATE);
    sf::WindowHandle windowHandle = m_renderWindow.getSystemHandle();
    ShowWindow(windowHandle, SW_MINIMIZE);
}

ConsoleViewController::~ConsoleViewController()
{
    //dtor
}

void ConsoleViewController::close(const Event& event)
{

}

void ConsoleViewController::resize(const Event& event)
{

}

void ConsoleViewController::leftButtonPressed(const Event& event)
{

}

void ConsoleViewController::midleButtonPressed(const Event& event)
{

}

void ConsoleViewController::rightButtonPressed(const Event& event)
{

}

void ConsoleViewController::leftButtonReleased(const Event& event)
{

}

void ConsoleViewController::midleButtonReleased(const Event& event)
{

}

void ConsoleViewController::rightButtonReleased(const Event& event)
{

}

void ConsoleViewController::mouseWheel(const Event& event)
{

}

void ConsoleViewController::mouseMove(const Event& event)
{

}

void ConsoleViewController::keyboardPressed(const Event& event)
{

}

void ConsoleViewController::keyboardReleased(const Event& event)
{

}

void ConsoleViewController::draw()
{
    m_fps.update();
    m_renderWindow.clear(sf::Color::Black);
    m_renderWindow.display();
}
