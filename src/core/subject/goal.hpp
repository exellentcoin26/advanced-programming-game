#pragma once

#ifndef GAME_SRC_SUBJECT_SUBJECT_GOAL_H
#define GAME_SRC_SUBJECT_SUBJECT_GOAL_H

#include "subject.hpp"

namespace subject {

/// Goal class representing the finish in the world.
class Goal : public Subject {
public:
    /// Creates a new `Goal` instance.
    inline Goal(const Vec2& pos, const Bounds& bounds) : Subject(pos, bounds) {}
};

}; // namespace subject

#endif // GAME_SRC_SUBJECT_SUBJECT_GOAL_H
