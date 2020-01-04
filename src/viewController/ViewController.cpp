#include "viewController/ViewController.h"

ViewController::ViewController(VideoMode& mode, const string& title, Uint32 style)
{
    m_renderWindow.create(mode, title, style, ContextSettings());
}

ViewController::~ViewController()
{
    //dtor
}
