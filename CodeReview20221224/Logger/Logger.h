// PHPのロギングmonologを理解する
// https://reffect.co.jp/php/monolog-to-understand


#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H


#include <variant>
#include <string>
#include <array>

#include "LogStruct.h"
#include "LogConsoleHandler.h"
#include "LogFileHandler.h"


namespace logger {
class Logger {
    using Handler = std::variant<LogConsoleHandler, LogFileHandler>;
private:
    //! ログハンドラの配列
    std::array<Handler, 2> _handlers;
    //! 出力時に低レベルへ出力する
    bool _is_output_to_low_level;
    //! すぐに出力する
    bool _immediate;
    //! _immediateがfalseの場合にログ情報を積み込む
    std::vector<LogStruct> _stack;
private:
    void Output(Level level, const char* file, int line, const char* message) {
        for(auto& handler : _handlers) {
            std::visit(
                [&](auto& handler) {
                    handler.Output(level, file, line, message);
                }, handler);
        } // for
    }
    void Log(Level level, const char* file, int line, const char* message) {
        if(_immediate) {
            this->Output(level, file, line, message);
        } // if
        else {
            _stack.push_back(LogStruct(
                level, file, line, message
            ));
        } // else
    }
public:
    Logger() :
        _handlers(),
        _is_output_to_low_level(false),
        _immediate(true),
        _stack() {
        _handlers = {
            LogFileHandler(),
            LogConsoleHandler()
        };
    }
    void Debug(const char* file, int line, const char* message) {
        Log(Level::Debug, file, line, message);
    }
    void Info(const char* file, int line, const char* message) {
        Log(Level::Info, file, line, message);
        if(this->_is_output_to_low_level) {
            this->Debug(file, line, message);
        } // if
    }
    void Notice(const char* file, int line, const char* message) {
        Log(Level::Notice, file, line, message);
        if(this->_is_output_to_low_level) {
            this->Info(file, line, message);
        } // if
    }
    void Warning(const char* file, int line, const char* message) {
        Log(Level::Warning, file, line, message);
        if(this->_is_output_to_low_level) {
            this->Notice(file, line, message);
        } // if
    }
    void Error(const char* file, int line, const char* message) {
        Log(Level::Error, file, line, message);
        if(this->_is_output_to_low_level) {
            this->Warning(file, line, message);
        } // if
    }
    void Critical(const char* file, int line, const char* message) {
        Log(Level::Critical, file, line, message);
        if(this->_is_output_to_low_level) {
            this->Error(file, line, message);
        } // if
    }
    void Alert(const char* file, int line, const char* message) {
        Log(Level::Alert, file, line, message);
        if(this->_is_output_to_low_level) {
            this->Critical(file, line, message);
        } // if
    }
    void Emergency(const char* file, int line, const char* message) {
        Log(Level::Emergency, file, line, message);
        if(this->_is_output_to_low_level) {
            this->Alert(file, line, message);
        } // if
    }

    void Flush(void) {
        _ASSERT(!this->_immediate);

        for(auto& log : _stack) {
            this->Output(log.level, log.file.c_str(), log.line, log.message.c_str());
        } // for
        _stack.clear();
    }
};
}
#endif // !LOGGER_LOGGER_H