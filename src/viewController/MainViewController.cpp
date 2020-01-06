#include "viewController/MainViewController.h"

MainViewController::MainViewController(VideoMode& mode, const string& title, Uint32 style) : ViewController(mode, title, style)
{
    m_renderWindow.setFramerateLimit(60);
    sf::View view(sf::FloatRect(0.f, 0.f, mode.width, mode.height));
    m_renderWindow.setView(view);
}

MainViewController::~MainViewController()
{
    //dtor
}

void MainViewController::update(vector<FunctionBlockListPtr>& functionBlockVector)
{
    m_functionBlockVector = functionBlockVector;
    prepareFunctionBlocks();
    lootAtMainFunctionalBlock();
}

void MainViewController::prepareFunctionBlocks()
{
    // get stages info (max height stage, stages width and height)
    uint64_t maxHeightStage {0};
    sf::Vector2u stagesInfo[m_functionBlockVector.size()];

    for(size_t stageListIndex {0}; stageListIndex < m_functionBlockVector.size(); ++stageListIndex)
    {
        FunctionBlockListPtr fbList = m_functionBlockVector[stageListIndex];
        for(FunctionBlock& fb : *fbList)
        {
            sf::Vector2u fbSize = fb.getSize();
            stagesInfo[stageListIndex].y += fbSize.y + ST_Y_GAP;
            if(fbSize.x > stagesInfo[stageListIndex].x)
                stagesInfo[stageListIndex].x = fbSize.x;
        }

        stagesInfo[stageListIndex].y -= ST_Y_GAP;

        if(stagesInfo[stageListIndex].y > maxHeightStage)
            maxHeightStage = stagesInfo[stageListIndex].y;
    }

    // set initial positions
    uint64_t xPosition {0};
    for(size_t stageListIndex {0}; stageListIndex < m_functionBlockVector.size(); ++stageListIndex)
    {
        FunctionBlockListPtr fbList = m_functionBlockVector[stageListIndex];

        uint64_t yPosition = (maxHeightStage - stagesInfo[stageListIndex].y) / 2;
        for(FunctionBlock& fb : *fbList)
        {
            fb.setPosition(xPosition, yPosition);
            yPosition += fb.getSize().y + ST_Y_GAP;
        }

        xPosition += stagesInfo[stageListIndex].x + ST_X_GAP;
    }
}

void MainViewController::drawStages()
{
    for(FunctionBlockListPtr fbListPtr : m_functionBlockVector)
    {
        for(FunctionBlock& fb : *fbListPtr)
        {
            m_renderWindow.draw(fb);
        }
    }
}

void MainViewController::lootAtMainFunctionalBlock()
{
    if(m_functionBlockVector.size() > 0)
    {
        FunctionBlockListPtr firstStage = m_functionBlockVector[0];
        if(firstStage->size() == 1)
        {
            FunctionBlock& mainFuntion = firstStage->front();

            sf::Vector2f fbPosition = mainFuntion.getPosition();
            sf::Vector2u fbSize = mainFuntion.getSize();
            sf::Vector2u windowSize = m_renderWindow.getSize();

            sf::View currentView{m_renderWindow.getView()};
            currentView.setCenter((windowSize.x - fbSize.x) / 2, fbPosition.y + 30);
            m_renderWindow.setView(currentView);
        }
    }
}

sf::Vector2f MainViewController::goToDefinition(sf::Vector2f clickPoint)
{
    sf::Vector2f pos;
    bool loopBreaker = false;
    list<FunctionBlock>::iterator findedElement;

    for(size_t stageIndex = 0; stageIndex < m_functionBlockVector.size() && !loopBreaker; ++stageIndex)
    {
        FunctionBlockListPtr functionBlockListPtr = m_functionBlockVector[stageIndex];
        for(list<FunctionBlock>::iterator iterator = functionBlockListPtr->begin(); iterator != functionBlockListPtr->end() && !loopBreaker; ++iterator)
        {
            FunctionBlock& functionBlock = *iterator;
            if(functionBlock.isContainsPoint(clickPoint))
            {
                loopBreaker = true;
                findedElement = iterator;
            }
        }
    }

    if(loopBreaker)
    {
        FunctionBlock& fb = *findedElement;
        string functionName = fb.getFunctionNameFromPoint(clickPoint);

        loopBreaker = false;
        for(size_t stageIndex = 0; stageIndex < m_functionBlockVector.size() && !loopBreaker; ++stageIndex)
        {
            FunctionBlockListPtr functionBlockListPtr = m_functionBlockVector[stageIndex];
            for(FunctionBlock functionBlock : *functionBlockListPtr)
            {
                if(functionBlock.getFunctionName().compare(functionName) == 0)
                {
                    sf::Vector2f functionBlockPos = functionBlock.getPosition();
                    sf::Vector2u functionBlockSize = functionBlock.getSize();
                    sf::VideoMode vMode = sf::VideoMode::getDesktopMode();

                    pos.x = functionBlockPos.x + (functionBlockSize.x / 2.0);
                    pos.y = functionBlockPos.y + (vMode.height / 2.0) - (ST_Y_GAP / 2.0);

                    loopBreaker = true;
                    break;
                }
            }
        }
    }

    return pos;
}

void MainViewController::close(const Event& event)
{
    m_renderWindow.close();
}

void MainViewController::resize(const Event& event)
{
    sf::View currentView{m_renderWindow.getView()};
    currentView.setSize(event.size.width, event.size.height);
    const sf::Vector2i currentWindowSize{resizeOldWindowSize.x - event.size.width, resizeOldWindowSize.y - event.size.height};
    currentView.move(-currentWindowSize.x, -currentWindowSize.y);
    resizeOldWindowSize.x = event.size.width;
    resizeOldWindowSize.y = event.size.height;
    m_renderWindow.setView(currentView);
}

void MainViewController::leftButtonPressed(const Event& event)
{
    leftMouseState = true;
    moveMousePressPoint.x = event.mouseButton.x;
    moveMousePressPoint.y = event.mouseButton.y;
    moveCenterViewPoint = m_renderWindow.getView().getCenter();
}

void MainViewController::midleButtonPressed(const Event& event)
{

}

void MainViewController::rightButtonPressed(const Event& event)
{
    if(!moveViewToDefinition)
    {
        sf::Vector2i mouseButtonReleasedPoint {event.mouseButton.x, event.mouseButton.y};
        sf::Vector2f pixel {m_renderWindow.mapPixelToCoords(mouseButtonReleasedPoint)};
        moveView2Pos = goToDefinition(pixel);
        if(moveView2Pos.x != 0 && moveView2Pos.y != 0)
        {
            sf::View currentView{m_renderWindow.getView()};
            sf::Vector2f viewCenterPos = currentView.getCenter();
            increaseDelta.x = (moveView2Pos.x - viewCenterPos.x) / 60.0;
            increaseDelta.y = (moveView2Pos.y - viewCenterPos.y) / 60.0;

            frameCounter = 0;
            moveViewToDefinition = true;
        }
    }
}

void MainViewController::leftButtonReleased(const Event& event)
{
    leftMouseState = false;
}

void MainViewController::midleButtonReleased(const Event& event)
{

}

void MainViewController::rightButtonReleased(const Event& event)
{

}

void MainViewController::mouseWheel(const Event& event)
{
    if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
    {
        float zoom {1.f + ZOOM_STEP};
        wheelCount++;

        if (event.mouseWheelScroll.delta > 0)
        {
            zoom = 1.f / zoom;
            wheelCount -= 2;
        }

        globalZoom = pow(zoom, abs(wheelCount));

        sf::Vector2i pixel{ event.mouseWheelScroll.x, event.mouseWheelScroll.y };
        const sf::Vector2f beforeCoord{m_renderWindow.mapPixelToCoords(pixel)};
        sf::View view{m_renderWindow.getView()};
        view.zoom(zoom);
        m_renderWindow.setView(view);
        const sf::Vector2f afterCoord{ m_renderWindow.mapPixelToCoords(pixel)};
        const sf::Vector2f offsetCoords{beforeCoord - afterCoord};
        view.move(offsetCoords);
        m_renderWindow.setView(view);
    }
}

void MainViewController::mouseMove(const Event& event)
{
    if(leftMouseState)
    {
        sf::View currentView{m_renderWindow.getView()};
        currentView.setCenter(moveCenterViewPoint.x - (event.mouseMove.x - moveMousePressPoint.x) * globalZoom,
                              moveCenterViewPoint.y - (event.mouseMove.y - moveMousePressPoint.y) * globalZoom);
        m_renderWindow.setView(currentView);
    }
}

void MainViewController::keyboardPressed(const Event& event)
{
    sf::View currentView = m_renderWindow.getView();
    sf::Vector2f centerPoint = currentView.getCenter();

    if(event.key.code == sf::Keyboard::Up)
        currentView.setCenter(centerPoint.x, centerPoint.y - 10.f);
    else if(event.key.code == sf::Keyboard::Right)
        currentView.setCenter(centerPoint.x - 10.f, centerPoint.y);
    else if(event.key.code == sf::Keyboard::Down)
        currentView.setCenter(centerPoint.x, centerPoint.y+10.f);
    else if(event.key.code == sf::Keyboard::Left)
        currentView.setCenter(centerPoint.x + 10.f, centerPoint.y);

    m_renderWindow.setView(currentView);
}

void MainViewController::keyboardReleased(const Event& event)
{

}

void MainViewController::draw()
{
    m_renderWindow.clear(sf::Color::Black);
    if(moveViewToDefinition)
    {
        if(frameCounter == 59)
            moveViewToDefinition = false;

        ++frameCounter;
        sf::View currentView{m_renderWindow.getView()};
        currentView.move(increaseDelta.x, increaseDelta.y);
        m_renderWindow.setView(currentView);
    }
    drawStages();
    m_renderWindow.display();
}
