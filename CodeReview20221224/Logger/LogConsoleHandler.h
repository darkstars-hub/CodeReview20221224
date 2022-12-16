#ifndef LOGGER_LOG_CONSOLE_HANDLER_H
#define LOGGER_LOG_CONSOLE_HANDLER_H


#include <iostream>

#include "LogFormatter.h"


namespace logger {
class LogConsoleHandler {
private:
    //! フォーマッタ
    LogFormatter _formatter;
public:
    void Output(Level level, const char* file, int line, const char* message) const {
        std::string string;
        _formatter.Format(level, file, line, message, string);
        std::cout << string;
    }
};
}
#endif //!LOGGER_LOG_CONSOLE_HANDLER_H