#include "state.h"

using namespace state;

State::State(Window* window) : window(window) {}

MenuState::MenuState(Window* window) : State(window) {}

void state::MenuState::update(Keyboard* keyboard) {}

OptionsState::OptionsState(Window* window) : State(window) {}

void state::OptionsState::update(Keyboard* keyboard) {}
