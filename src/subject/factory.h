#pragma once

#ifndef GAME_SRC_SUBJECT_FACTORY_H
#define GAME_SRC_SUBJECT_FACTORY_H

#include "../math/vec.h"
#include "entity/entity.h"
#include "goal.h"
#include "subject.h"
#include "tile.h"

/// Abstract subject factory classed used to create subjects with correct view/renderer attached
class SubjectFactory {
protected:
    using Subject = subject::Subject;
    using Entity = subject::entity::Entity;
    using Player = subject::entity::Player;
    using Goal = subject::Goal;
    using Tile = subject::Tile;

    using Bounds = subject::Bounds;
    using Vec2 = math::Vec2;

public:
    SubjectFactory() = default;
    virtual ~SubjectFactory() = default;

    /* This looks like I should use an enum, but the idea is that not all functions should take the same arguments */

    /// Construct subject and return pointer.
    virtual Subject* create_subject(std::shared_ptr<Camera> cam, const Vec2& pos, const Bounds& bounds) const = 0;

    /// Construct entity and return pointer.
    virtual Entity* create_entity(std::shared_ptr<Camera> cam, const Vec2& pos, const Bounds& bounds) const = 0;

    /// Construct player and return pointer.
    virtual Player* create_player(std::shared_ptr<Camera> cam, const Vec2& pos, const Bounds& bounds) const = 0;

    /// Construct goal and return pointer.
    virtual Goal* create_goal(std::shared_ptr<Camera> cam, const Vec2& pos, const Bounds& bounds) const = 0;

    /// Construct tile and return pointer.
    virtual Tile* create_tile(std::shared_ptr<Camera> cam, const Vec2& pos, const Bounds& bounds) const = 0;
};

#endif // GAME_SRC_SUBJECT_FACTORY_H
