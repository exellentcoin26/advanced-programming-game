#pragma once

#ifndef GAME_SRC_STATE_STATEMANAGER_H
#define GAME_SRC_STATE_STATEMANAGER_H

#include "controllers/keyboard.hpp"
#include "core/utils/log.hpp"
#include "state.hpp"

#include <stdexcept>
#include <unordered_map>

namespace state {

class State;

class StateManager {
public:
    enum class StateType {
        MenuState,
        OptionsState,
        GameState,
        Default = MenuState,
    };

private:
    using Keyboard = controls::Keyboard;

private:
    class StateManagerBuilder {
    public:
        inline StateManagerBuilder(StateType start_state) : start_state_type(start_state) {}

        inline StateManagerBuilder& insert_state(StateType state_type, State* state) {
            this->states[state_type] = std::unique_ptr<State>(state);

            return *this;
        }

        inline StateManager* build() {
            // check if start state is a valid state
            if (this->states.find(this->start_state_type) == this->states.end())
                throw std::runtime_error("cannot create StateManager when start state type is not a defined state");

            return new StateManager{this->start_state_type, std::move(this->states)};
        }

    private:
        std::unordered_map<StateType, std::unique_ptr<State>> states{};
        const StateType start_state_type;
    };

public:
    static StateManagerBuilder create_state_manager(StateType start_state) { return {start_state}; }

    inline StateManager& operator=(StateManager&& other) {
        this->states = std::move(other.states);
        this->current_state = other.current_state;

        return *this;
    }

    inline void change_state(StateType state) {
        if (this->states.find(state) == this->states.end())
            throw std::runtime_error("cannot switch state to state that is not defined");

        this->current_state = state;
    }

    void update(Keyboard* keyboard);

    inline StateManager& insert_state(StateType state_type, State* state) {
        this->states[state_type] = std::unique_ptr<State>(state);
        return *this;
    }

private:
    inline StateManager(StateType start_state, std::unordered_map<StateType, std::unique_ptr<State>>&& states)
        : states(std::move(states)), current_state(start_state) {}

private:
    std::unordered_map<StateType, std::unique_ptr<State>> states;
    StateType current_state;
};

} // namespace state

#endif // GAME_SRC_STATE_STATEMANAGER_H
