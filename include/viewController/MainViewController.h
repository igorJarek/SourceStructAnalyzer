#ifndef MAINVIEWCONTROLLER_H
#define MAINVIEWCONTROLLER_H

#include <SFML/Graphics.hpp>
#include <WindowsManager.h>
#include <viewController/ViewController.h>
#include <Typedefs.h>
#include <FunctionBlock.h>
#include <AnimationMove.h>

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

        virtual void keyboardPressed(const Event& event);
        virtual void keyboardReleased(const Event& event);

        virtual void draw();

        void update(vector<FunctionBlockListPtr>& functionBlockVector);

    private:
        void prepareFunctionBlocks();
        void drawStages();
        void lootAtMainFunctionalBlock();

        pair<size_t, FunctionBlock*> getFunctionBlockFromPoint(sf::Vector2f clickPoint);
        pair<size_t, FunctionBlock*> getFunctionBlockByName(const string& functionName);

        vector<FunctionBlockListPtr> m_functionBlockVector;

        sf::Vector2u resizeOldWindowSize {m_renderWindow.getSize()};
        sf::Vector2i moveMousePressPoint;
        sf::Vector2f moveCenterViewPoint;
        bool leftMouseState {false};
        int64_t wheelCount {0};
        double globalZoom {1.0};

        AnimationMove m_animationMove;
};

#endif // MAINVIEWCONTROLLER_H
