#include "gamestate.h"

#include "../utils/log.h"

#include <iostream>
#include <optional>

using namespace state;

GameState::GameState(std::shared_ptr<Window> window)
    : State(window), world([&]() -> World {
          level::LevelInfo level_info = level::load_level_from_file("assets/levels/level0.toml");
          return World(level_info, std::make_shared<SFMLSubjectFactory>(window));
      }()) {}

void GameState::update(Keyboard* keyboard) {
    std::optional<std::set<Input>> input{};

    if (keyboard->is_key_down(Keyboard::Key::A)) {
        if (!input.has_value())
            input = std::set<Input>{};

        input->insert(Input::Left);
    }
    if (keyboard->is_key_down(Keyboard::Key::D)) {
        if (!input.has_value())
            input = std::set<Input>{};

        input->insert(Input::Right);
    }
    if (keyboard->is_key_pressed(Keyboard::Key::Space)) {
        if (!input.has_value())
            input = std::set<Input>{};

        input->insert(Input::Jump);
    }

    if (input.has_value())

        this->world.move_player(input.value());

    this->world.update();
}
