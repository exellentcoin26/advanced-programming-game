#include "state.h"

#include "controllers/resource_manager.h"

#include <cassert>

using namespace state;

State::State(std::shared_ptr<Window> window, std::shared_ptr<StateManager> state_manager)
    : window(window), state_manager(state_manager) {}

MenuState::MenuState(std::shared_ptr<Window> window, std::shared_ptr<StateManager> state_manager, u32 selector_amount)
    : State(window, state_manager), selector_amount(selector_amount), font([&]() -> sf::Font {
          sf::Font font;
          if (!font.loadFromFile("assets/font.ttf"))
              throw std::runtime_error("failed to load font");
          return font;
      }()) {}

void state::MenuState::update(Keyboard* keyboard) {
    const f64 gap_percent = 0.55;

    auto resource_manager = ResourceManager::get_instance();

    // check keyboard input
    if (keyboard->is_key_pressed(Keyboard::Key::S) && this->selected != resource_manager->get_levels().size() - 1) {
        ++this->selected;

        if (this->selected - this->offset >= this->selector_amount)
            ++this->offset;
    }
    if (keyboard->is_key_pressed(Keyboard::Key::W) && this->selected != 0) {
        --this->selected;

        if (static_cast<int>(this->selected) - static_cast<int>(this->offset) < 0)
            --this->offset;
    }
    if (keyboard->is_key_pressed(Keyboard::Key::Space) || keyboard->is_key_pressed(Keyboard::Key::Enter)) {
        const bool result = resource_manager->set_current_level(this->selected);

        assert((result && "resource manager succeeded in setting the selected level"));

        state_manager->change_state(StateManager::StateType::GameState);
        resource_manager->set_current_level(this->selected);
    }

    // render the menu
    const f64 gap_size = this->window->getSize().y / static_cast<f64>(this->selector_amount) * gap_percent;
    const f64 block_size_x = this->window->getSize().x * 0.8;
    const f64 block_size_y =
        (this->window->getSize().y - gap_size * (this->selector_amount + 1)) / static_cast<f64>(this->selector_amount);
    const f64 block_offset = (this->window->getSize().x - block_size_x) / 2.0;

    for (u32 i = this->offset;
         i < std::min(this->selector_amount + this->offset, static_cast<u32>(resource_manager->get_levels().size()));
         ++i) {
        sf::RectangleShape box{};
        box.setFillColor((i == this->selected) ? sf::Color::White : sf::Color::Black);
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(4.0);
        box.setPosition(
            sf::Vector2f(block_offset, (i - this->offset + 1) * gap_size + (i - this->offset) * block_size_y));
        box.setSize(sf::Vector2f(block_size_x, block_size_y));

        sf::Text text(resource_manager->get_levels().at(i).first.name +
                          ((resource_manager->get_levels().at(i).second) ? " V" : ""),
                      this->font);
        text.setFillColor((i == this->selected) ? sf::Color::Black : sf::Color::White);
        text.setPosition(
            sf::Vector2f((block_offset + block_size_x / 2.0) - (text.getLocalBounds().width / 2.0),
                         ((i - this->offset + 1) * gap_size + (i - this->offset) * block_size_y + block_size_y / 2.0) -
                             (text.getLocalBounds().height / 2.0)));

        this->window->draw(box);
        this->window->draw(text);
    }
}

OptionsState::OptionsState(std::shared_ptr<Window> window, std::shared_ptr<StateManager> state_manager)
    : State(window, state_manager) {}

void state::OptionsState::update(Keyboard* keyboard) { (void)keyboard; }
