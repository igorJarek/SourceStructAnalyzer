#ifndef ANIMATIONMOVE_H
#define ANIMATIONMOVE_H

#include <SFML/Graphics.hpp>

class AnimationMove
{
    public:
        AnimationMove() {}
        AnimationMove(sf::Vector2f sourcePosition, sf::Vector2f destinationPosition, uint16_t frameRate, float time);
        ~AnimationMove();

        bool isActive()             const   { return m_active; }
        void disable()                      { m_active = false; }

        void increaseCounter()              { ++m_frameCounter; }
        uint16_t getFrameCounter()  const   { return m_frameCounter; }
        uint16_t getFrameCutOff()   const   { return m_frameCutOff; }

        sf::Vector2f getDelta()     const   { return m_delta; }

    private:
        bool m_active                       {false};

        sf::Vector2f m_delta;
        uint16_t m_frameCounter             {0};
        uint16_t m_frameCutOff              {0};
};

#endif // ANIMATIONMOVE_H
