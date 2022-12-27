#pragma once

#ifndef GAME_SRC_SUBJECT_TILE_H
#define GAME_SRC_SUBJECT_TILE_H

#include "subject.hpp"

namespace subject {

class Tile : public Subject {
public:
    inline Tile(const Vec2& pos, const Bounds& bounds) : Subject(pos, bounds) {}
    virtual ~Tile() = default;
};

} // namespace subject

#endif // GAME_SRC_SUBJECT_TILE_H
