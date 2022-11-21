#pragma once

#ifndef GAME_SRC_UTILS_LOG_H
#define GAME_SRC_UTILS_LOG_H

#include <memory>
#include <ostream>

namespace utils {

class LogStream : public std::ostream {
public:
    LogStream() = default;
    LogStream(bool ignore, std::ostream& os);

    template <class T>
    inline std::ostream& operator<<(const T& t) {
        return (os) ? (*os) << t : *this;
    }

private:
    /// if set the stream will ignore input
    std::ostream* os;
};

/// Singleton logger/tracing class
class Logger {
public:
    enum class LogLevel { None = 0, Error = 1, Warn = 2, Info = 3, Debug = 4, Default = Debug };

    static std::shared_ptr<Logger> get_instance() {
        if (!instance) {
            instance = std::make_shared<Logger>();
        }

        return instance;
    }

    inline void config(LogLevel level, std::ostream& out) {
        this->level = level;
        this->out = &out;
    }

    inline std::unique_ptr<LogStream> log(const std::string& file, unsigned int line, const std::string& function,
                                          LogLevel level) const {
        const bool ignore = (level > this->level);
        auto logstream = std::make_unique<utils::LogStream>(ignore, *this->out);

        const char* level_string = log_level_strings[static_cast<int>(level) - 1];

        *(logstream) << '[' << level_string << "][" << file << ':' << line << "][" << function << "] ";

        return logstream;
    }

private:
    static std::shared_ptr<Logger> instance;
    std::ostream* out;

    LogLevel level;

    static const char* log_level_strings[];
};

} // namespace utils

#define LOG(level) *utils::Logger::get_instance()->log(__FILE__, __LINE__, __func__, utils::Logger::LogLevel::level)

#endif // GAME_SRC_UTILS_LOG_H
