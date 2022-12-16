#ifndef LOGGER_LOG_FILE_HANDLER_H
#define LOGGER_LOG_FILE_HANDLER_H


#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "LogLevel.h"
#include "LogFormatter.h"


namespace logger {
class LogFileHandler {
private:
    //! ファイル
    std::unordered_map<Level, std::ofstream > _fstreams;
    //! フォーマッタ
    LogFormatter _formatter;
    //! 出力先ディレクトリ
    std::string _directory_path = "";
private:
    bool IsExistDirectory(const char* path) const noexcept {
        std::error_code error;

        bool exist = std::filesystem::exists(path, error);
        if(!exist) {
            std::cout << error.message() << std::endl;
            return false;
        } // if

        return std::filesystem::is_directory(path, error);
    }
    bool MakeDirectory(const char* directory) const noexcept {
        if(this->IsExistDirectory(directory)) {
            return false;
        } // if

        std::error_code error;
        (void)std::filesystem::create_directories(directory, error);
        std::cout << error.message() << std::endl;

        return this->IsExistDirectory(directory);
    }

    //! str = level-localdate
    void FileName(Level level, std::string& str) const noexcept {
        std::string levelstr;
        ToString(level, levelstr);
        levelstr += "-";


        std::string time;// = "2022-12-24";
        auto utc_now = std::chrono::system_clock::now();
        auto tz = std::chrono::locate_zone(kLocateZone);
        auto jst_now = tz->to_local(utc_now);

        std::stringstream ss;
        ss << jst_now;
        time = ss.str();
        auto it = time.find(" ");
        time = time.substr(0, it);

        str = levelstr + time;
    }

    void EmplaceStream(void) {
        for(int i = 0; i < static_cast<int>(Level::Count); i++) {
            _fstreams.emplace(
                static_cast<Level>(i),
                decltype(_fstreams)::mapped_type());
        } // for
    }
public:
    LogFileHandler() :
        _fstreams(),
        _formatter(),
        _directory_path("../Log/") {
        this->EmplaceStream();
    }
    LogFileHandler(const char* dirctory_path) :
        _fstreams(),
        _formatter(),
        _directory_path(dirctory_path) {
        this->EmplaceStream();
    }
    LogFileHandler(const LogFileHandler& other) :
        _fstreams(),
        _formatter(),
        _directory_path(other._directory_path) {
        this->EmplaceStream();
    }
    LogFileHandler& operator=(const LogFileHandler& other) {
        return *this;
    }
    ~LogFileHandler() {
        for(auto& fstream : _fstreams) {
            if(fstream.second) {
                fstream.second.close();
            } // if
        } // for
    }
    void Output(Level level, const char* file, int line, const char* message) {
        std::string string;
        _formatter.Format(level, file, line, message, string);

        if(!this->IsExistDirectory(_directory_path.c_str())) {
            _ASSERT(this->MakeDirectory(_directory_path.c_str()));
        } // if

        auto& stream = _fstreams.at(level);
        if(!stream.is_open()) {
            std::string filename;
            this->FileName(level, filename);

            auto filepath = _directory_path + filename + ".log";
            if(std::filesystem::exists(filepath)) {
                stream.open(filepath, std::ios::app);
            } // if
            else {
                stream.open(filepath, std::ios::out);
            } // else
        } // if
        stream << string;
    }
};
}
#endif //!LOGGER_LOG_FILE_HANDLER_H