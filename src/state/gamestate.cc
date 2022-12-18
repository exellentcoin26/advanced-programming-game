#include "gamestate.h"

#include "controllers/resource_manager.h"
#include "utils/log.h"

#include <iostream>
#include <optional>

using namespace state;

GameState::GameState(std::shared_ptr<Window> window, std::shared_ptr<SubjectFactory> factory)
    : State(window), world([&]() -> World {
          auto resource_manager = ResourceManager::get_instance();
          return World(resource_manager->get_current_level().first, factory);
      }()), factory(factory) {}

void GameState::update(Keyboard* keyboard) {
    // check that the world is completed or failed
    std::pair<bool, bool> completed_or_failed = this->world.completed_or_failed();
    auto resource_manager = ResourceManager::get_instance();
    if (completed_or_failed.first) {
        // try advance the current level
        bool success = resource_manager->set_current_level(resource_manager->get_current_level_index() + 1);

        if (!success) {
            // find a level that has not been completed yet.
            const auto levels = resource_manager->get_levels();
            bool found{false};
            for (unsigned int i = 0; i < levels.size(); ++i) {
                if (levels.at(i).second)
                    continue;

                // set new current level
                resource_manager->set_current_level(i);
                found = true;
                break;
            }

            if (!found) {
                // exit game with success
                std::cout << "All levels completed. Congratulations!\n";
                exit(0);
            }
        }

        // load world with found level
        this->world = World(resource_manager->get_current_level().first, this->factory);
    }

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

    // check if current world

    this->world.update();
}
