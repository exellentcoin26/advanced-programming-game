#pragma once

#ifndef GAME_SRC_SUBJECT_TILE_H
#define GAME_SRC_SUBJECT_TILE_H

#include "subject.hpp"

namespace subject {

/// Tile class representing a wall in the `World`
class Tile : public Subject {
public:
    /// Creates a new `Tile` instance.
    inline Tile(const Vec2& pos, const Bounds& bounds) : Subject(pos, bounds) {}
    virtual ~Tile() = default;
};

} // namespace subject

#endif // GAME_SRC_SUBJECT_TILE_H
