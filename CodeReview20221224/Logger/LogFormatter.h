#ifndef LOGGER_LOG_FORMATTER_H
#define LOGGER_LOG_FORMATTER_H


#include <chrono>
#include <sstream>

#include "LogDefine.h"
#include "LogLevel.h"


namespace logger {
class LogFormatter {
public:
    void Format(Level level, const char* file, int line, const char* message, std::string& out) const noexcept {
        std::stringstream ss;

        auto utc_now = std::chrono::system_clock::now();
        auto tz = std::chrono::locate_zone(kLocateZone);
        auto jst_now = tz->to_local(utc_now);
        auto jst_info = tz->get_info(jst_now);

        std::string levelstr;
        ToString(level, levelstr);

        ss << "[" << levelstr << "]" << " ";
        ss << "[" << utc_now << " " << "UTC" << "]" << " " << "[" << jst_now << " " << jst_info.first.abbrev << "]" << kCRLF;
        ss << file << "-" << line << "." << kCRLF;
        ss << message << kCRLF;

        out = ss.str();
    }
};
}
#endif //!LOGGER_LOG_FORMATTER_H