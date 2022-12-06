#pragma once

#ifndef GAME_SRC_STATE_STATEMANAGER_H
#define GAME_SRC_STATE_STATEMANAGER_H

#include "../controllers/keyboard.h"
#include "../utils/log.h"
#include "state.h"

#include <stdexcept>
#include <unordered_map>

namespace state {

template <typename StateType>
class StateManager {
private:
    using Keyboard = controls::Keyboard;

    using State = state::State;

private:
    class StateManagerBuilder {
    public:
        StateManagerBuilder(StateType start_state) : start_state_type(start_state) {}

        StateManagerBuilder& insert_state(StateType state_type, State* state) {
            this->states[state_type] = std::unique_ptr<State>(state);

            return *this;
        }

        StateManager build() {
            // check if start state is a valid state
            if (this->states.find(this->start_state_type) == this->states.end())
                throw std::runtime_error("cannot create StateManager when start state type is not a defined state");

            return {this->start_state_type, std::move(this->states)};
        }

    private:
        std::unordered_map<StateType, std::unique_ptr<State>> states{};
        const StateType start_state_type;
    };

public:
    static StateManagerBuilder create_state_manager(StateType start_state) { return {start_state}; }

    StateManager& operator=(StateManager&& other) {
        this->states = std::move(other.states);
        this->current_state = other.current_state;
    }

    void change_state(StateType state) {
        if (this->states.find(state) == this->states.end())
            throw std::runtime_error("cannot switch state to state that is not defined");

        this->current_state = state;
    }

    void update(Keyboard* keyboard) { this->states.at(this->current_state)->update(keyboard); }

private:
    StateManager(StateType start_state, std::unordered_map<StateType, std::unique_ptr<State>>&& states)
        : states(std::move(states)), current_state(start_state) {}

private:
    const std::unordered_map<StateType, std::unique_ptr<State>> states;
    StateType current_state;
};

} // namespace state

#endif // GAME_SRC_STATE_STATEMANAGER_H
