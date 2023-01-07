#pragma once

#ifndef GAME_SRC_STATE_GAMESTATE_H
#define GAME_SRC_STATE_GAMESTATE_H

#include "SFML/Graphics.hpp"

#include "state.hpp"
#include "statemanager.hpp"

#include "../factory.hpp"
#include "core/world.hpp"

namespace state {

class GameState : public State {
private:
    enum class GameStateStateType {
        LevelSelectorState,
        LevelState,
    };

public:
    GameState(std::shared_ptr<Window> window, std::shared_ptr<StateManager> state_manager,
              std::shared_ptr<SubjectFactory> factory);
    ~GameState() = default;

    void update(Keyboard* keyboard) override;

private:
    World world;
    std::shared_ptr<SubjectFactory> factory;
};

} // namespace state

#endif // GAME_SRC_STATE_GAMESTATE_H
