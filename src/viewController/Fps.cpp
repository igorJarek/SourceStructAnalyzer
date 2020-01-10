#include "Fps.h"

Fps::Fps(sf::RenderWindow& renderWindow, const std::string& title) : m_renderWindow(renderWindow), m_title(title)
{
    m_timeStart = std::chrono::high_resolution_clock::now();
    m_frameCounter = 0;
}

Fps::~Fps()
{
    //dtor
}

void Fps::update()
{
    m_timeCurrent = std::chrono::high_resolution_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(m_timeCurrent - m_timeStart).count() <= 1000)
        ++m_frameCounter;
    else
    {
        m_renderWindow.setTitle(m_title + "    FPS : " + std::to_string(m_frameCounter)); // \t doesn't work
        m_frameCounter = 0;
        m_timeStart = std::chrono::high_resolution_clock::now();
    }
}
