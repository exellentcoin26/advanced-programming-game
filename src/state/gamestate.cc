#include "gamestate.h"

#include "../utils/log.h"

state::LevelSelectorState::~LevelSelectorState() {}
void state::LevelSelectorState::update(Keyboard* keyboard) {}

state::LevelState::~LevelState() {}
void state::LevelState::update(Keyboard* keyboard) {}

state::GameState::GameState()
    : state_manager(StateManager::create_state_manager(GameStateStateType::LevelState)
                        .insert_state(GameStateStateType::LevelState, new LevelState())
                        .insert_state(GameStateStateType::LevelSelectorState, new LevelSelectorState())
                        .build()) {}

void state::GameState::update(Keyboard* keyboard) {}
