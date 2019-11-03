#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

#define Log Logger::get()

class Logger
{
    private:
        Logger();
        Logger(const Logger&);
        ~Logger();

        std::fstream fileStream;
    public:
        static Logger& get()
        {
            static Logger instance;
            return instance;
        }

        template<typename T>
        Logger& operator<<(const T& value)
        {
            fileStream << value;
            return *this;
        }

        constexpr static const char* endl{"\n"};
};

#endif // LOGGER_H
