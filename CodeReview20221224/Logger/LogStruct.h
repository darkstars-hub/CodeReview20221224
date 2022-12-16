#ifndef LOGGER_LOG_STRUCT_H
#define LOGGER_LOG_STRUCT_H


#include <string>

#include "LogLevel.h"


namespace logger {
struct LogStruct {
    //! ログレベル
    Level level;
    //! __FILE__
    std::string file;
    //! __LINE__
    int line;
    //! ログメッセージ
    std::string message;

    LogStruct(Level level, const char* file, int line, const char* message) :
        level(level),
        file(file),
        line(line),
        message(message) {
    }
};
}
#endif //!LOGGER_LOG_STRUCT_H