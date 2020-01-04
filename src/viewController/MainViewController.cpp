#include "viewController/MainViewController.h"

MainViewController::MainViewController(VideoMode& mode, const string& title, Uint32 style) : ViewController(mode, title, style)
{
    m_renderWindow.setFramerateLimit(60);
}

MainViewController::~MainViewController()
{
    //dtor
}

void MainViewController::close(const Event& event)
{
    m_renderWindow.close();
}

void MainViewController::resize(const Event& event)
{

}

void MainViewController::leftButtonPressed(const Event& event)
{

}

void MainViewController::midleButtonPressed(const Event& event)
{

}

void MainViewController::rightButtonPressed(const Event& event)
{

}

void MainViewController::leftButtonReleased(const Event& event)
{

}

void MainViewController::midleButtonReleased(const Event& event)
{

}

void MainViewController::rightButtonReleased(const Event& event)
{

}

void MainViewController::mouseWheel(const Event& event)
{

}

void MainViewController::mouseMove(const Event& event)
{

}
