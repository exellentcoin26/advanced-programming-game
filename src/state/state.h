#pragma once

#ifndef GAME_SRC_STATE_STATE_H
#define GAME_SRC_STATE_STATE_H

#include "../utils/keyboard.h"
#include "../utils/log.h"

#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace state {

class State {
protected:
    using Keyboard = utils::Keyboard;

public:
    State() = default;
    virtual ~State() = default;

    virtual void update(Keyboard* keyboard) = 0;
};

class MenuState : public State {
public:
    MenuState() = default;
    ~MenuState() override;

    void update(Keyboard* keyboard) override;
};

class OptionsState : public State {
public:
    OptionsState() = default;
    ~OptionsState() override;

    void update(Keyboard* keyboard) override;
};

} // namespace state

#endif // GAME_SRC_STATE_H
