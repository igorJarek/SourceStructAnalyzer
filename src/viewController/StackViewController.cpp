#include "StackViewController.h"

StackViewController::StackViewController(VideoMode& mode, const string& title, Uint32 style) : ViewController(mode, title, style)
{
    m_renderWindow.setFramerateLimit(60);
}

StackViewController::~StackViewController()
{
    //dtor
}

void StackViewController::close(const Event& event)
{
    m_renderWindow.close();
}

void StackViewController::resize(const Event& event)
{

}

void StackViewController::leftButtonPressed(const Event& event)
{

}

void StackViewController::midleButtonPressed(const Event& event)
{

}

void StackViewController::rightButtonPressed(const Event& event)
{

}

void StackViewController::leftButtonReleased(const Event& event)
{

}

void StackViewController::midleButtonReleased(const Event& event)
{

}

void StackViewController::rightButtonReleased(const Event& event)
{

}

void StackViewController::mouseWheel(const Event& event)
{

}

void StackViewController::mouseMove(const Event& event)
{

}

void StackViewController::keyboardPressed(const Event& event)
{

}

void StackViewController::keyboardReleased(const Event& event)
{

}

void StackViewController::draw()
{
    m_renderWindow.clear(sf::Color::Green);
    m_renderWindow.display();
}
