#include "gamestate.h"

#include "controllers/resource_manager.h"
#include "utils/log.h"

#include <iostream>
#include <optional>

using namespace state;

GameState::GameState(std::shared_ptr<Window> window, std::shared_ptr<StateManager> state_manager,
                     std::shared_ptr<SubjectFactory> factory)
    : State(window, state_manager), world([&]() -> World {
          auto resource_manager = ResourceManager::get_instance();
          return World(resource_manager->get_current_level().first, factory);
      }()),
      factory(factory) {}

void GameState::update(Keyboard* keyboard) {
    // check that the world is completed or failed
    std::pair<bool, bool> completed_or_failed = this->world.completed_or_failed();
    auto resource_manager = ResourceManager::get_instance();
    if (this->world.get_index() != resource_manager->get_current_level_index()) {
        // initialize level
        this->world = World(resource_manager->get_current_level().first, this->factory,
                            resource_manager->get_current_level_index());
    }
    if (completed_or_failed.first) {
        // mark level as completed
        resource_manager->get_current_level_mut().second = true;

        // try advance the current level
        bool success = resource_manager->set_current_level(resource_manager->get_current_level_index() + 1);

        if (!success) {
            // find a level that has not been completed yet.
            const auto levels = resource_manager->get_levels();
            bool found{false};
            for (u32 i = 0; i < levels.size(); ++i) {
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
        this->world = World(resource_manager->get_current_level().first, this->factory,
                            resource_manager->get_current_level_index());
    } else if (completed_or_failed.second) {
        this->world = World(resource_manager->get_current_level().first, this->factory,
                            resource_manager->get_current_level_index());
    }

    if (keyboard->is_key_pressed(Keyboard::Key::Escape)) {
        this->state_manager->change_state(StateManager::StateType::MenuState);
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
