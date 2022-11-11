#pragma once

#ifndef GAME_SRC_GAME_H
#define GAME_SRC_GAME_H

#include "SFML/Graphics.hpp"

#include "utils/log.h"
#include "utils/stopwatch.h"
#include "utils/types.h"

#include <string>

class Game {
private:
    using Window = sf::RenderWindow;
    using StopWatch = utils::StopWatch;

    using SystemClock = std::chrono::system_clock;
    using TimePoint = std::chrono::time_point<SystemClock>;

public:
    /// Returns a new game instance with vsync set to `fps`.
    Game(u32 width, u32 height, const std::string& title, u32 fps);

    // Returns a new game instance with vsync turned off.
    Game(u32 width, u32 height, const std::string& title);

    /// Starts the main game loop and launches the window.
    void start();

private:
    Window* create_window(u32 width, u32 height, const std::string& title) const;

private:
    const u32 width, height;
    std::unique_ptr<Window> window;

    const u32 fps{};
    const bool vsync{};
};

#endif // GAME_SRC_GAME_H
