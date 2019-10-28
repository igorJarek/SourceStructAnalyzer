#ifndef VERSION_H_INCLUDED
#define VERSION_H_INCLUDED

#include <string>

using namespace std;

static const uint32_t VERSION_MAJOR    = 1;
static const uint32_t VERSION_MINOR    = 0;
static const uint32_t VERSION_BUGFIX   = 0;

static const string VERSION_STRING = string(to_string(VERSION_MAJOR)) + "." + \
                                     string(to_string(VERSION_MINOR)) + "." + \
                                     string(to_string(VERSION_BUGFIX));
#endif // VERSION_H_INCLUDED
