#include "Logger.h"

Logger::Logger()
{
    fileStream.open("log.txt", std::fstream::out | std::fstream::trunc);
    if(!fileStream.is_open())
    {
        // TODO
    }
}

Logger::~Logger()
{
    if(fileStream.is_open())
        fileStream.close();
}
