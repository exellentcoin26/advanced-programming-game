#pragma once

#ifndef GAME_SRC_STATE_STATE_H
#define GAME_SRC_STATE_STATE_H

#include "SFML/Graphics.hpp"

#include "controllers/keyboard.h"
#include "statemanager.h"
#include "utils/types.h"

#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace state {

class StateManager;

class State {
protected:
    using Keyboard = controls::Keyboard;
    using Window = sf::RenderWindow;

public:
    State(std::shared_ptr<Window> window, std::shared_ptr<StateManager> state_manager);
    virtual ~State() = default;

    virtual void update(Keyboard* keyboard) = 0;

protected:
    std::shared_ptr<Window> window;
    std::shared_ptr<StateManager> state_manager;
};

class MenuState : public State {
public:
    MenuState(std::shared_ptr<Window> window, std::shared_ptr<StateManager> state_manager, u32 selector_amount = 5);
    ~MenuState() = default;

    void update(Keyboard* keyboard) override;

private:
    /// Only show `x` amount of levels, starting from the offset.
    u32 offset{0};
    const u32 selector_amount;

    u32 selected{0};

    const sf::Font font;
};

class OptionsState : public State {
public:
    OptionsState(std::shared_ptr<Window> window, std::shared_ptr<StateManager> state_manager);
    ~OptionsState() = default;

    void update(Keyboard* keyboard) override;
};

} // namespace state

#endif // GAME_SRC_STATE_STATE_H
