#pragma once

#ifndef GAME_SRC_STATE_GAMESTATE_H
#define GAME_SRC_STATE_GAMESTATE_H

#include "SFML/Graphics.hpp"

#include "state.h"
#include "statemanager.h"

#include "../factory.h"
#include "../world.h"

namespace state {

class GameState : public State {
private:
    enum class GameStateStateType {
        LevelSelectorState,
        LevelState,
    };

private:
    using StateManager = state::StateManager<GameStateStateType>;

public:
    GameState(std::shared_ptr<Window> window, std::shared_ptr<SubjectFactory> factory);
    ~GameState() = default;

    void update(Keyboard* keyboard) override;

private:
    World world;
    std::shared_ptr<SubjectFactory> factory;
};

} // namespace state

#endif // GAME_SRC_STATE_GAMESTATE_H
