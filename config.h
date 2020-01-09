#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <string>
#include <SFML/Graphics/Color.hpp>

const std::string FONT_PATH = "courier-new.ttf";
const float ZOOM_STEP {0.15f}; // 15 % zoom per "one revolution :)"

// Functional Block Config
const uint16_t FB_CHAR_SIZE                 {11};
const uint16_t FB_PADDING                   {6};
const uint16_t FB_BORDER_THICKNESS          {4};
const uint16_t FB_GAP_BETWEEN_PATH_AND_NAME {10};
const uint16_t FB_GAP_BETWEEN_NAME_AND_CODE {16};
const int16_t FB_GAP_BETWEEN_CODE_AND_CODE  {-2};

const sf::Color FB_DETECTED_FUNCTION_COLOR  {255, 50, 50};
const sf::Color FB_PATH_COLOR               {80, 80, 255};
const sf::Color FB_NAME_COLOR               {0, 128, 64};

// Display Stages Config
const uint16_t STAGE_X_GAP {40};
const uint16_t STAGE_Y_GAP {20};

// MainViewController
const uint16_t MAIN_VIEW_FRAMERATE              {60};
const uint16_t STACK_VIEW_FRAMERATE             {60};
const uint16_t CONSOLE_VIEW_FRAMERATE           {60};

// StackViewController
const uint16_t STACK_X_START                    {10};
const uint16_t STACK_Y_START                    {10};
const uint16_t STACK_CHAR_SIZE                  {12};
const uint16_t STACK_GAP_BETWEEN_CODE_AND_CODE  {5};

#endif // CONFIG_H_INCLUDED
