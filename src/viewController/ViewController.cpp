#include "viewController/ViewController.h"

ViewController::ViewController(VideoMode& mode, const string& title, Uint32 style)
{
    m_renderWindow.create(mode, title, style, ContextSettings());
}

ViewController::~ViewController()
{
    //dtor
}

void ViewController::setIcon(const string& file)
{
    if(m_iconImage.loadFromFile(file))
    {
        const uint8_t* pixels = m_iconImage.getPixelsPtr();
        Vector2u iconSize = m_iconImage.getSize();
        m_renderWindow.setIcon(iconSize.x, iconSize.y, pixels);
    }
}
