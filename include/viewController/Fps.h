#ifndef FPS_H
#define FPS_H

#include <SFML/Graphics.hpp>
#include <chrono>

class Fps
{
    public:
        Fps(sf::RenderWindow& renderWindow, const std::string& title);
        ~Fps();

        void update();

    private:
        using TimePoint = std::chrono::high_resolution_clock::time_point;

        sf::RenderWindow& m_renderWindow;
        const std::string m_title;

        uint16_t m_frameCounter {0};
        TimePoint m_timeStart;
        TimePoint m_timeCurrent;
};

#endif // FPS_H
