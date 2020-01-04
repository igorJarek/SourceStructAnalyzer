#include <SFML/Graphics.hpp>
#include <cmath>

#include "version.h"

#include <ProcessFlow.h>
#include <RowedFile.h>
#include <FunctionBlock.h>

int main(int argc, char *argv[])
{
    sf::VideoMode vMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(vMode.width, vMode.height), "Source Structure Analyzer " + VERSION_STRING);
    window.setFramerateLimit(60);

    sf::View view(sf::FloatRect(0.f, 0.f, vMode.width, vMode.height));
    window.setView(view);

    sf::Vector2u resizeOldWindowSize {window.getSize()};
    sf::Vector2i moveMousePressPoint {};
    sf::Vector2f moveCenterViewPoint {};
    bool leftMouseState = false;
    int64_t wheelCount {0};
    double globalZoom {1.0};

    bool moveViewToDefinition {false};
    sf::Vector2f moveView2Pos;
    sf::Vector2f increaseDelta;
    uint32_t frameCounter {0};

    ProcessFlow processFlow{argc, argv};
    processFlow.recursiveFolderSearch(processFlow.getExeFolderPath());
    processFlow.openMainFile();
    processFlow.iteratesCallsQueue();
    processFlow.prepareFunctionBlocks();
    processFlow.lootAtMainFunctionalBlock(window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::Resized)
            {
                sf::View currentView{window.getView()};
                currentView.setSize(event.size.width, event.size.height);
                const sf::Vector2i currentWindowSize{resizeOldWindowSize.x - event.size.width, resizeOldWindowSize.y - event.size.height};
                currentView.move(-currentWindowSize.x, -currentWindowSize.y);
                resizeOldWindowSize.x = event.size.width;
                resizeOldWindowSize.y = event.size.height;
                window.setView(currentView);
            }

            if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    leftMouseState = true;
                    moveMousePressPoint.x = event.mouseButton.x;
                    moveMousePressPoint.y = event.mouseButton.y;
                    moveCenterViewPoint = window.getView().getCenter();
                }
                else if(event.mouseButton.button == sf::Mouse::Right)
                {
                    if(!moveViewToDefinition)
                    {
                        sf::Vector2i mouseButtonReleasedPoint {event.mouseButton.x, event.mouseButton.y};
                        sf::Vector2f pixel {window.mapPixelToCoords(mouseButtonReleasedPoint)};
                        moveView2Pos = processFlow.goToDefinition(pixel);
                        if(moveView2Pos.x != 0 && moveView2Pos.y != 0)
                        {
                            sf::View currentView{window.getView()};
                            sf::Vector2f viewCenterPos = currentView.getCenter();
                            increaseDelta.x = (moveView2Pos.x - viewCenterPos.x) / 60.0;
                            increaseDelta.y = (moveView2Pos.y - viewCenterPos.y) / 60.0;

                            frameCounter = 0;
                            moveViewToDefinition = true;
                        }
                    }
                }
            }

            if(event.type == sf::Event::MouseButtonReleased)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                    leftMouseState = false;
            }

            if(event.type == sf::Event::MouseMoved)
            {
                if(leftMouseState)
                {
                    sf::View currentView{window.getView()};
                    currentView.setCenter(moveCenterViewPoint.x - (event.mouseMove.x - moveMousePressPoint.x) * globalZoom,
                                          moveCenterViewPoint.y - (event.mouseMove.y - moveMousePressPoint.y) * globalZoom);
                    window.setView(currentView);
                }
            }

            if(event.type == sf::Event::MouseWheelScrolled)
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
                    const sf::Vector2f beforeCoord{window.mapPixelToCoords(pixel)};
                    sf::View view{window.getView()};
                    view.zoom(zoom);
                    window.setView(view);
                    const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel)};
                    const sf::Vector2f offsetCoords{beforeCoord - afterCoord};
                    view.move(offsetCoords);
                    window.setView(view);
                }
            }

            if(event.type == sf::Event::KeyPressed)
            {
                sf::View currentView = window.getView();
                sf::Vector2f centerPoint = currentView.getCenter();

                if(event.key.code == sf::Keyboard::Up)
                    currentView.setCenter(centerPoint.x, centerPoint.y - 10.f);
                else if(event.key.code == sf::Keyboard::Right)
                    currentView.setCenter(centerPoint.x - 10.f, centerPoint.y);
                else if(event.key.code == sf::Keyboard::Down)
                    currentView.setCenter(centerPoint.x, centerPoint.y+10.f);
                else if(event.key.code == sf::Keyboard::Left)
                    currentView.setCenter(centerPoint.x + 10.f, centerPoint.y);

                window.setView(currentView);
            }
        }

        window.clear(sf::Color::Black);
        if(moveViewToDefinition)
        {
            if(frameCounter == 59)
                moveViewToDefinition = false;

            ++frameCounter;
            sf::View currentView{window.getView()};
            currentView.move(increaseDelta.x, increaseDelta.y);
            window.setView(currentView);
        }
        processFlow.drawStages(window);
        window.display();
    }

    return 0;
}
