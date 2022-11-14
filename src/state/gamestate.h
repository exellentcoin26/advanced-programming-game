#pragma once

#ifndef GAME_SRC_STATE_GAMESTATE_H
#define GAME_SRC_STATE_GAMESTATE_H

#include "state.h"
#include "statemanager.h"

#include <unordered_map>

namespace state {

class LevelSelectorState : public State {
public:
    LevelSelectorState() = default;
    ~LevelSelectorState();

    void update(Keyboard* keyboard) override;
};

class LevelState : public State {
public:
    LevelState() = default;
    ~LevelState();

    void update(Keyboard* keyboard) override;
};

class GameState : public State {
private:
    enum class GameStateStateType {
        LevelSelectorState,
        LevelState,
    };

private:
    using StateManager = state::StateManager<GameStateStateType>;

public:
    GameState();
    ~GameState() = default;

    void update(Keyboard* keyboard) override;

private:
    StateManager state_manager;
};

} // namespace state

#endif // GAME_SRC_STATE_GAMESTATE_H