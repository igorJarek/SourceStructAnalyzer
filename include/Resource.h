#ifndef RESOURCE_H
#define RESOURCE_H

#include <SFML/Graphics.hpp>
#include <../config.h>

class Resource
{
    private:
        Resource();
        Resource(const Resource&);

        sf::Font funcBlockFont;

    public:
        static Resource& instance()
        {
            static Resource instance;
            return instance;
        }

        sf::Font& getFuncBlockFont() { return funcBlockFont; }
};

#endif // RESOURCE_H
