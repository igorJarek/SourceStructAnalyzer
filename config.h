#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <string>

const std::string FONT_PATH = "courier-new.ttf";
const float ZOOM_STEP {0.1f}; // 10 % zoom per "one click :)"

// Functional Block Config
const int FB_CHAR_SIZE {10};
const int FB_PADDING {6};
const int FB_BORDER_THICKNESS {4};

// Display Stages Config
const int ST_X_GAP {30};
const int ST_Y_GAP {15};

#endif // CONFIG_H_INCLUDED
