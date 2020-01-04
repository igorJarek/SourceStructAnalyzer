#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class ViewController
{
    public:
        ViewController(VideoMode& mode, const string& title, Uint32 style);
        ~ViewController();

        virtual void close(const Event& event) = 0;
        virtual void resize(const Event& event) = 0;

        virtual void leftButtonPressed(const Event& event) = 0;
        virtual void midleButtonPressed(const Event& event) = 0;
        virtual void rightButtonPressed(const Event& event) = 0;

        virtual void leftButtonReleased(const Event& event) = 0;
        virtual void midleButtonReleased(const Event& event) = 0;
        virtual void rightButtonReleased(const Event& event) = 0;

        virtual void mouseWheel(const Event& event) = 0;
        virtual void mouseMove(const Event& event) = 0;

        RenderWindow& getWindow() { return m_renderWindow; }

    protected:
        RenderWindow m_renderWindow;
};

#endif // VIEWCONTROLLER_H
