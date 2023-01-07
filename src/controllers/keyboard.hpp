#pragma once

#ifndef GAME_SRC_UTILS_KEYBOARD_H
#define GAME_SRC_UTILS_KEYBOARD_H

#include "SFML/Window/Keyboard.hpp"

#include <bitset>

namespace controls {

/// Keyboard listener class
class Keyboard {
public:
    /// Reexport of `sf::Keyboard::Key` enum class
    using Key = sf::Keyboard::Key;

public:
    /// Returns a new Keyboard listener instance
    Keyboard();

    /// Updates the intern state of the keyboard listener
    void update();

    /// Returns the current state of the key
    bool is_key_down(Key key) const;

    /// Returns whether the key is pressed in this update
    bool is_key_pressed(Key key) const;

    /// Returns wether the key is released in this update
    bool is_key_released(Key key) const;

private:
    std::bitset<Key::KeyCount> key_down{};
    std::bitset<Key::KeyCount> key_pressed{};
    std::bitset<Key::KeyCount> key_released{};
    std::bitset<Key::KeyCount> can_press;
};

} // namespace controls

#endif // GAME_SRC_UTILS_KEYBOARD_H
