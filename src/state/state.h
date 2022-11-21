#pragma once

#ifndef GAME_SRC_STATE_STATE_H
#define GAME_SRC_STATE_STATE_H

#include "SFML/Graphics.hpp"

#include "../utils/keyboard.h"
#include "../utils/log.h"

#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace state {

class State {
protected:
    using Keyboard = utils::Keyboard;
    using Window = sf::RenderWindow;

public:
    State(Window* window);
    virtual ~State() = default;

    virtual void update(Keyboard* keyboard) = 0;

protected:
    Window* window;
};

class MenuState : public State {
public:
    MenuState(Window* window);
    ~MenuState() = default;

    void update(Keyboard* keyboard) override;
};

class OptionsState : public State {
public:
    OptionsState(Window* window);
    ~OptionsState() = default;

    void update(Keyboard* keyboard) override;
};

} // namespace state

#endif // GAME_SRC_STATE_H
