#ifndef MAINVIEWCONTROLLER_H
#define MAINVIEWCONTROLLER_H

#include <viewController/ViewController.h>

class MainViewController : public ViewController
{
    public:
        MainViewController(VideoMode& mode, const string& title, Uint32 style = Style::Default);
        ~MainViewController();

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

    private:
};

#endif // MAINVIEWCONTROLLER_H
