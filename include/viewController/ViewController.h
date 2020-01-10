#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <SFML/Graphics.hpp>
#include <Fps.h>

using namespace std;
using namespace sf;

class ViewController
{
    public:
        ViewController(VideoMode& mode, const string& title, Uint32 style);
        ~ViewController();

        bool isVisible() const { return m_visibility; }
        void setVisibility(bool visibility);

        void setIcon(const string& file);

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

        virtual void keyboardPressed(const Event& event) = 0;
        virtual void keyboardReleased(const Event& event) = 0;

        virtual void draw() = 0;

        RenderWindow& getWindow() { return m_renderWindow; }

    protected:
        RenderWindow m_renderWindow;
        const string m_title;
        Image m_iconImage;
        Fps m_fps;

        bool m_visibility {true};
};

#endif // VIEWCONTROLLER_H
