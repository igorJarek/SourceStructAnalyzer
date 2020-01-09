#include "StackViewController.h"

StackViewController::StackViewController(VideoMode& mode, const string& title, Uint32 style) : ViewController(mode, title, style)
{
    m_renderWindow.setFramerateLimit(STACK_VIEW_FRAMERATE);
    sf::WindowHandle windowHandle = m_renderWindow.getSystemHandle();
    ShowWindow(windowHandle, SW_MINIMIZE);
}

StackViewController::~StackViewController()
{
    //dtor
}

void StackViewController::addTextData(const string& functionBlockName, size_t functionStagePosition, const string& clickedFunction, size_t clickedFunctionStagePosition)
{
    static uint32_t yPos {STACK_Y_START};
    string text {functionBlockName + " [S : " + to_string(functionStagePosition) + "] => " + clickedFunction + " [S : " + to_string(clickedFunctionStagePosition) + "]"};
    uint32_t firstOpenBracketPos    = text.find("[");
    uint32_t firstCloseBracketPos   = text.find("]");
    uint32_t equalPos               = text.find("=");
    uint32_t secondOpenBracketPos   = text.find("[", equalPos);
    uint32_t secondCloseBracketPos  = text.find("]", equalPos);

    TextExt textElement(text, Resource::instance().getFuncBlockFont(), STACK_CHAR_SIZE);
    textElement.setPosition(STACK_X_START, yPos);
    textElement.changeCharactersColor(FB_NAME_COLOR, Pos(0, functionBlockName.size()), 0);
    textElement.changeCharactersColor(sf::Color::Red, Pos(firstOpenBracketPos, firstCloseBracketPos), 0);
    textElement.changeCharactersColor(FB_PATH_COLOR, Pos(equalPos, equalPos + 1), 0);
    textElement.changeCharactersColor(sf::Color::Red, Pos(secondOpenBracketPos, secondCloseBracketPos), 0);

    sf::FloatRect floatRect = textElement.getLocalBounds();
    yPos += floatRect.height + STACK_GAP_BETWEEN_CODE_AND_CODE;
    m_textExtVector.push_back(textElement);
}

void StackViewController::close(const Event& event)
{

}

void StackViewController::resize(const Event& event)
{
    sf::View view(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
    m_renderWindow.setView(view);
}

void StackViewController::leftButtonPressed(const Event& event)
{

}

void StackViewController::midleButtonPressed(const Event& event)
{

}

void StackViewController::rightButtonPressed(const Event& event)
{

}

void StackViewController::leftButtonReleased(const Event& event)
{

}

void StackViewController::midleButtonReleased(const Event& event)
{

}

void StackViewController::rightButtonReleased(const Event& event)
{

}

void StackViewController::mouseWheel(const Event& event)
{

}

void StackViewController::mouseMove(const Event& event)
{

}

void StackViewController::keyboardPressed(const Event& event)
{

}

void StackViewController::keyboardReleased(const Event& event)
{

}

void StackViewController::draw()
{
    m_renderWindow.clear(sf::Color::Black);

    for(TextExt& textExt : m_textExtVector)
        m_renderWindow.draw(textExt);

    m_renderWindow.display();
}
