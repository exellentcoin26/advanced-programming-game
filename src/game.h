#pragma once

#ifndef GAME_SRC_GAME_H
#define GAME_SRC_GAME_H

#include "SFML/Graphics.hpp"

#include "state/gamestate.h"
#include "state/state.h"
#include "state/statemanager.h"
#include "utils/keyboard.h"
#include "utils/log.h"
#include "utils/stopwatch.h"
#include "utils/types.h"

#include <optional>
#include <stdexcept>
#include <string>

/// Main game class running the game loop.
class Game {
public:
    enum class WindowStyle {
        Windowed,
        FullScreen,
        BorderLess,
        Default = FullScreen,
    };

private:
    enum class StateType {
        MenuState,
        OptionsState,
        GameState,
        Default = MenuState,
    };

private:
    using Window = sf::RenderWindow;
    using Keyboard = utils::Keyboard;
    using StopWatch = utils::StopWatch;

    using SystemClock = std::chrono::system_clock;
    using TimePoint = std::chrono::time_point<SystemClock>;

    using State = state::State;
    using StateManager = state::StateManager<StateType>;

    using GameState = state::GameState;
    using MenuState = state::MenuState;
    using OptionsState = state::OptionsState;

private:
    /// Game builder class used to configure `Game`.
    class GameBuilder {
    public:
        GameBuilder(WindowStyle style, std::pair<u32, u32> size, const std::string& title, bool vsync);

        /// Sets the fps if vsync is set to true.
        GameBuilder& set_fps(u32 fps);

        /// Returns a new instance of `Game` with the provided config.
        Game build() const;

    private:
        const WindowStyle style;
        const std::pair<u32, u32> size;
        const std::string title;

        const bool vsync;
        std::optional<u32> fps{};
    };

public:
    /// Creates a new `GameBuilder` instance to further configure a `Game` instance.
    static GameBuilder create_game(WindowStyle style, u32 width, u32 height, const std::string& title, bool vsync);

    /// Starts the main game loop.
    void start();

private:
    Game(WindowStyle style, u32 width, u32 height, const std::string& title, u32 fps, bool vsync);

    Window* create_window(u32 width, u32 height, const std::string& title) const;

private:
    const WindowStyle style;
    const u32 width, height;
    std::shared_ptr<Window> window;

    const u32 fps;
    const bool vsync{};

    StateManager state_manager;
    Keyboard keyboard;
};

#endif // GAME_SRC_GAME_H
