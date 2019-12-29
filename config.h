#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <string>
#include <SFML/Graphics/Color.hpp>

const std::string FONT_PATH = "courier-new.ttf";
const float ZOOM_STEP {0.15f}; // 15 % zoom per "one click :)"

// Functional Block Config
const uint16_t FB_CHAR_SIZE                 {11};
const uint16_t FB_PADDING                   {6};
const uint16_t FB_BORDER_THICKNESS          {4};
const uint16_t FB_GAP_BETWEEN_PATH_AND_NAME {10};
const uint16_t FB_GAP_BETWEEN_NAME_AND_CODE {16};
const int16_t FB_GAP_BETWEEN_CODE_AND_CODE  {-2};

const sf::Color FB_DETECTED_FUNCTION_COLOR  {255, 50, 50};

// Display Stages Config
const int ST_X_GAP {30};
const int ST_Y_GAP {15};

#endif // CONFIG_H_INCLUDED
