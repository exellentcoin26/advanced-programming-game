#include "gamestate.h"

#include "../utils/log.h"

#include <optional>

using namespace state;

LevelSelectorState::LevelSelectorState(Window* window) : State(window) {}

void LevelSelectorState::update(Keyboard* keyboard) {}

LevelState::LevelState(Window* window) : State(window), world(World(SFMLSubjectFactory(window))) {}

void state::LevelState::update(Keyboard* keyboard) {
    std::optional<Input> input;
    if (keyboard->is_key_down(Keyboard::Key::A))
        input = Input::Left;
    else if (keyboard->is_key_down(Keyboard::Key::D))
        input = Input::Right;
    else if (keyboard->is_key_pressed(Keyboard::Key::Space))
        input = Input::Jump;

    if (input.has_value())
        this->world.move_player(input.value());
    this->world.update();
}

GameState::GameState(Window* window)
    : State(window),
      state_manager(StateManager::create_state_manager(GameStateStateType::LevelState)
                        .insert_state(GameStateStateType::LevelState, new LevelState(window))
                        .insert_state(GameStateStateType::LevelSelectorState, new LevelSelectorState(window))
                        .build()) {}

void GameState::update(Keyboard* keyboard) { this->state_manager.update(keyboard); }
