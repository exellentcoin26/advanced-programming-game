#pragma once

#ifndef GAME_SRC_SUBJECT_TILE_H
#define GAME_SRC_SUBJECT_TILE_H

#include "subject.h"

namespace subject {

class Tile : public Subject {
public:
    inline Tile() : Subject({0, 0}, Bounds({0, 0}, {4.0, 4.0})) {}
    virtual ~Tile() = default;

    inline void update() override {}
};

} // namespace subject

#endif // GAME_SRC_SUBJECT_TILE_H
