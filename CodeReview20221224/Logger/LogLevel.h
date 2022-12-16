#ifndef LOGGER_LOG_LEVEL_H
#define LOGGER_LOG_LEVEL_H


#include "ToString.h"


namespace logger {
enum class Level {
    Debug, Info, Notice, Warning, Error, Critical, Alert, Emergency, Count
};
template<>
inline void logger::ToString<Level>(Level level, std::string& out) {
    switch(level) {
        case Level::Debug:
            out = "Debug";
            break;
        case Level::Info:
            out = "Info";
            break;
        case Level::Notice:
            out = "Notice";
            break;
        case Level::Warning:
            out = "Warning";
            break;
        case Level::Error:
            out = "Error";
            break;
        case Level::Critical:
            out = "Critical";
            break;
        case Level::Alert:
            out = "Alert";
            break;
        case Level::Emergency:
            out = "Emergency";
            break;
        case Level::Count:
            out = "Count";
            break;
        default:
            break;
    } // switch
}
}
#endif // !LOGGER_LOG_LEVEL_H