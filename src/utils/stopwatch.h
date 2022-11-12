#pragma once

#ifndef GAME_SRC_UTILS_STOPWATCH_H
#define GAME_SRC_UTILS_STOPWATCH_H

#include "types.h"

#include <chrono>
#include <memory>

namespace utils {

// Singleton stopwatch class
class StopWatch {
private:
    using SystemClock = std::chrono::system_clock;
    using TimePoint = std::chrono::time_point<SystemClock>;
    using Duration = std::chrono::duration<double>;

public:
    /// Creates a new stopwatch instance (Should not be called by user)
    inline StopWatch() : start_time(SystemClock::now()) {}

    /// Returns the singleton `Stopwatch` instance.
    inline static std::shared_ptr<StopWatch> get_instance() {
        if (!instance) {
            instance = std::make_shared<StopWatch>();
        }
        return instance;
    }

    /// Stops the current counter and stores delta time between now and last start.
    inline void stop() {
        this->delta_time =
            std::chrono::duration_cast<std::chrono::nanoseconds>(SystemClock::now() - this->start_time).count() / 1.0e9;
    }

    /// Returns the delta time (in seconds) between last stop and reset/start call
    inline f64 get_delta_time() { return this->delta_time; }

    /// Resets the stopwatch
    inline void reset_and_start() { this->start_time = SystemClock::now(); }

private:
    static std::shared_ptr<StopWatch> instance;

    TimePoint start_time;
    f64 delta_time{};
};

} // namespace utils

#endif // GAME_SRC_UTILS_STOPWATCH_H
