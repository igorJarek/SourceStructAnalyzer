#ifndef STACKVIEWCONTROLLER_H
#define STACKVIEWCONTROLLER_H

#include <viewController/ViewController.h>
#include <windows.h>
#include <TextExt.h>
#include <Resource.h>

class StackViewController : public ViewController
{
    public:
        StackViewController(VideoMode& mode, const string& title, Uint32 style = Style::Default);
        ~StackViewController();

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

        void addTextData(const string& functionBlockName, size_t functionStagePosition, const string& clickedFunction, size_t clickedFunctionStagePosition);

    private:
        vector<TextExt> m_textExtVector;
};

#endif // STACKVIEWCONTROLLER_H
