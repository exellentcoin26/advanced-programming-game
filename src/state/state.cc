#include "state.h"

using namespace state;

State::State(std::shared_ptr<Window> window) : window(window) {}

MenuState::MenuState(std::shared_ptr<Window> window) : State(window) {}

void state::MenuState::update(Keyboard* keyboard) { (void)keyboard; }

OptionsState::OptionsState(std::shared_ptr<Window> window) : State(window) {}

void state::OptionsState::update(Keyboard* keyboard) { (void)keyboard; }
