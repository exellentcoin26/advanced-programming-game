#pragma once

#ifndef GAME_SRC_SUBJECT_SUBJECT_GOAL_H
#define GAME_SRC_SUBJECT_SUBJECT_GOAL_H

#include "subject.hpp"

namespace subject {

class Goal : public Subject {
public:
    inline Goal(const Vec2& pos, const Bounds& bounds) : Subject(pos, bounds) {}
};

}; // namespace subject

#endif // GAME_SRC_SUBJECT_SUBJECT_GOAL_H
