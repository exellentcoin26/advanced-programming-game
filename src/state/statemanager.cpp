#include "statemanager.hpp"

using namespace state;

void StateManager::update(Keyboard* keyboard) { this->states.at(this->current_state)->update(keyboard); }
