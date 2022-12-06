#pragma once

#ifndef GAME_SRC_STATE_STATE_H
#define GAME_SRC_STATE_STATE_H

#include "SFML/Graphics.hpp"

#include "../controllers/keyboard.h"
#include "../utils/log.h"

#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace state {

class State {
protected:
    using Keyboard = controls::Keyboard;
    using Window = sf::RenderWindow;

public:
    State(std::shared_ptr<Window> window);
    virtual ~State() = default;

    virtual void update(Keyboard* keyboard) = 0;

protected:
    std::shared_ptr<Window> window;
};

class MenuState : public State {
public:
    MenuState(std::shared_ptr<Window> window);
    ~MenuState() = default;

    void update(Keyboard* keyboard) override;
};

class OptionsState : public State {
public:
    OptionsState(std::shared_ptr<Window> window);
    ~OptionsState() = default;

    void update(Keyboard* keyboard) override;
};

} // namespace state

#endif // GAME_SRC_STATE_H
