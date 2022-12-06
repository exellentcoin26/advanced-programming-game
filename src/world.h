#pragma once

#ifndef GAME_SRC_WORLD_H
#define GAME_SRC_WORLD_H

#include "controllers/keyboard.h"
#include "math/vec.h"
#include "utils/types.h"

#include "camera.h"
#include "subject/entity/entity.h"
#include "subject/factory.h"
#include "subject/goal.h"
#include "subject/subject.h"
#include "subject/tile.h"
#include "utils/levelloader.h"

#include <map>
#include <memory>
#include <set>

enum class Input {
    Left,
    Right,
    Jump,
};

class World {
private:
    using Subject = subject::Subject;
    using Entity = subject::entity::Entity;
    using Goal = subject::Goal;
    using Player = subject::entity::Player;
    using Bounds = subject::Bounds;
    using Tile = subject::Tile;

    using Vec2 = math::Vec2;

private:
    const f64 GRAVITY{0.1f};
    const f64 MOVEMENT_FORCE{0.1f};
    const f64 JUMP_FORCE{1.0f};

public:
    World(const level::LevelInfo& level_info, std::shared_ptr<SubjectFactory> factory);
    ~World() = default;

    /// Updates the physics and goal-detection.
    void update();

    /// Tries to move player using input
    void move_player(const std::set<Input>& input);

private:
    /// Checks collision with all entities per axis and updates the internal state of the entity
    void entity_check_collision(usize e_idx, const Bounds& old_bounds, const Bounds& new_bounds_x,
                                const Bounds& new_bounds_y);

private:
    /// camera that defines the view of the world
    std::shared_ptr<Camera> camera{};

    /// list of all subjects in this world
    std::map<usize, std::unique_ptr<Subject>> subjects{};

    /// list of all indexes that are entities
    std::set<usize> entities{};

    /// pointer to player in `subjects` map
    usize player{};
    /// pointer to goal in `subjects` map
    usize goal{};

    /// contiains textures
    std::shared_ptr<SubjectFactory> factory;
};

#endif // GAME_SRC_WORLD_H
