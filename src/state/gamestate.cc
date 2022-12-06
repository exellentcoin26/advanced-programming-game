#include "gamestate.h"

#include "../utils/log.h"

#include <optional>

using namespace state;

LevelSelectorState::LevelSelectorState(std::shared_ptr<Window> window) : State(window) {}

void LevelSelectorState::update(Keyboard* keyboard) {}

LevelState::LevelState(std::shared_ptr<Window> window)
    : State(window), world([&]() -> World {
          level::LevelInfo level_info = level::load_level_from_file("tmp.toml");

          return World(level_info, std::make_shared<SFMLSubjectFactory>(window));
      }()) {}

void state::LevelState::update(Keyboard* keyboard) {
    std::optional<std::set<Input>> input{};
    if (keyboard->is_key_down(Keyboard::Key::A)) {
        if (!input.has_value())
            input = std::set<Input>{};
        input->insert(Input::Left);
    } else if (keyboard->is_key_down(Keyboard::Key::D)) {
        if (!input.has_value())
            input = std::set<Input>{};
        input->insert(Input::Right);
    } else if (keyboard->is_key_pressed(Keyboard::Key::Space)) {
        if (!input.has_value())
            input = std::set<Input>{};
        input->insert(Input::Jump);
    }

    if (input.has_value())
        this->world.move_player(input.value());
    this->world.update();
}

GameState::GameState(std::shared_ptr<Window> window)
    : State(window),
      state_manager(StateManager::create_state_manager(GameStateStateType::LevelState)
                        .insert_state(GameStateStateType::LevelState, new LevelState(window))
                        .insert_state(GameStateStateType::LevelSelectorState, new LevelSelectorState(window))
                        .build()) {}

void GameState::update(Keyboard* keyboard) { this->state_manager.update(keyboard); }
