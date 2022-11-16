#pragma once

#ifndef GAME_SRC_UTILS_ARRAY_H
#define GAME_SRC_UTILS_ARRAY_H

#include "types.h"

#include <memory>
#include <optional>
#include <stdexcept>
#include <variant>
#include <vector>

namespace utils {

namespace gen_vec {

struct GenIndex {
    usize index;
    usize generation;
};

/// Vector implemented using generation indeces.
/// Heavily inspired by (this
/// article)[lucassardois.medium.com/generational-indices-guide-8e3c5f7fd594]
template <typename T>
class GenVec {
private:
    struct Free {
        usize next_free;
    };

    struct GenEntry {
        std::variant<T, Free> entry;
        usize generation;
    };

public:
    GenVec() = default;
    ~GenVec() = default;

    GenIndex insert(T value) {
        GenIndex key;
        if (this->entries.size() > this->free_head) {
            if (std::holds_alternative<Free>(this->entries.at(this->free_head).entry)) {
                // replace free location with entry
                auto& [entry, generation] = this->entries.at(this->free_head);
                const auto& [next_free] = std::get<Free>(entry);

                key = {this->free_head, generation};

                this->free_head = next_free;
                entry = std::variant<T, Free>(value);
                return key;
            } else {
                // the free head points to an occupied entry
                throw std::runtime_error("corrupt GenVec, next free points to occupied spot");
            }
        } else {
            key = {this->entries.size(), 0};
            const GenEntry entry{value, 0};
            this->entries.push_back(entry);
            this->free_head = key.index + 1;
        }

        ++this->_size;
        return key;
    }

    std::optional<std::reference_wrapper<T>> get(GenIndex key) {
        if (this->entries.size() > key.index && std::holds_alternative<T>(this->entries.at(key.index).entry)) {
            auto& [entry, generation] = this->entries.at(key.index);

            if (generation == key.generation)
                return std::optional(std::ref(std::get<T>(entry)));
        }
        return std::nullopt;
    }

    void remove(GenIndex key) {
        auto& [entry, generation] = this->entries[key.index];

        if (std::holds_alternative<T>(entry)) {
            if (generation != key.generation)
                return;

            ++generation;
            entry = Free{this->free_head};
            this->free_head = key.index;
            --this->_size;
        }
    }

    usize size() { return this->_size; }

private:
    std::vector<GenEntry> entries{};
    usize free_head{};
    usize _size{};
};

} // namespace gen_vec

} // namespace utils

#endif // GAME_SRC_UTILS_ARRAY_H
