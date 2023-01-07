#include "keyboard.hpp"

#include "core/utils/types.hpp"

using namespace controls;

Keyboard::Keyboard() {
    // set all bits to 1
    this->can_press.set();
}

void Keyboard::update() {
    // check the current state for every key in the enum
    for (u32 i = 0; i < Key::KeyCount; ++i) {
        // reset the value for `key_pressed` and `key_released`
        this->key_pressed.reset(i);
        this->key_released.reset(i);

        const bool new_state = sf::Keyboard::isKeyPressed(static_cast<Key>(i));

        if (new_state == this->key_down.test(i))
            continue;

        if (new_state) {
            this->key_down.set(i);
            if (this->can_press.test(i)) {
                // key is pressed and the key is not held down
                this->key_pressed.set(i);
                this->can_press.reset(i);
            }
        } else {
            this->key_down.reset(i);
            if (!this->can_press.test(i)) {
                this->key_released.set(i);
                this->can_press.set(i);
            }
        }
    }
}

bool Keyboard::is_key_down(Key key) const { return this->key_down.test(static_cast<u32>(key)); }
bool Keyboard::is_key_pressed(Key key) const { return this->key_pressed.test(static_cast<u32>(key)); }
bool Keyboard::is_key_released(Key key) const { return this->key_released.test(static_cast<u32>(key)); }
