#include "Resource.h"

Resource::Resource()
{

    if (!funcBlockFont.loadFromFile(FONT_PATH))
        exit(0);
}
