#ifndef CONSOLEVIEWCONTROLLER_H
#define CONSOLEVIEWCONTROLLER_H

#include <viewController/ViewController.h>
#include <windows.h>

class ConsoleViewController : public ViewController
{
    public:
        ConsoleViewController(VideoMode& mode, const string& title, Uint32 style = Style::Default);
        ~ConsoleViewController();

        virtual void close(const Event& event);
        virtual void resize(const Event& event);

        virtual void leftButtonPressed(const Event& event);
        virtual void midleButtonPressed(const Event& event);
        virtual void rightButtonPressed(const Event& event);

        virtual void leftButtonReleased(const Event& event);
        virtual void midleButtonReleased(const Event& event);
        virtual void rightButtonReleased(const Event& event);

        virtual void mouseWheel(const Event& event);
        virtual void mouseMove(const Event& event);

        virtual void keyboardPressed(const Event& event);
        virtual void keyboardReleased(const Event& event);

        virtual void draw();

    private:
};

#endif // CONSOLEVIEWCONTROLLER_H
