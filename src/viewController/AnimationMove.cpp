#include "AnimationMove.h"

AnimationMove::AnimationMove(sf::Vector2f sourcePosition, sf::Vector2f destinationPosition, uint16_t frameRate, float time)
{
    m_active = true;
    m_frameCounter = 0;
    m_frameCutOff = frameRate * time;

    m_delta.x = destinationPosition.x - sourcePosition.x;
    m_delta.y = destinationPosition.y - sourcePosition.y;

    m_delta.x /= time;
    m_delta.y /= time;

    m_delta.x /= frameRate;
    m_delta.y /= frameRate;
}

AnimationMove::~AnimationMove()
{
    //dtor
}
