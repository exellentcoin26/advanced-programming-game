#pragma once

#ifndef GAME_SRC_WORLD_H
#define GAME_SRC_WORLD_H

#include "math/vec.hpp"
#include "utils/types.hpp"

#include "camera.hpp"
#include "core/utils/levelloader.hpp"
#include "subject/entity/entity.hpp"
#include "subject/factory.hpp"
#include "subject/goal.hpp"
#include "subject/subject.hpp"
#include "subject/tile.hpp"

#include <map>
#include <memory>
#include <set>

/// Enum of input that can be sent to move the `Player`.
enum class Input {
    Left,
    Right,
    Jump,
};

/// Main `World` class containing all subjects (eg. `Player`, `Wall`, `Goal`)
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
    const f64 GRAVITY{3.5f};
    const f64 MOVEMENT_FORCE{7.0f};
    const f64 JUMP_FORCE{1.8f};

public:
    /// Creates a new `World` instance from the `level_info` and uses `factory` to create `Subjects`.
    /// Note: index is only used by the level manager.
    World(const level::LevelInfo& level_info, const std::shared_ptr<SubjectFactory>& factory, usize index = 0);
    ~World() = default;

    World(World&& world) = default;
    World& operator=(World&& world);

    /// Updates the physics and goal-detection.
    void update();

    /// Tries to move player using `input`.
    void move_player(const std::set<Input>& input);

    /// Returns `{completed, failed}` object.
    inline std::pair<bool, bool> completed_or_failed() const { return {this->finished, this->dead}; }

    /// Returns the stored index of the leve
    inline usize get_index() const { return this->index; }

private:
    /// Checks collision with all entities per axis and updates the internal state of the entity.
    void entity_check_collision_and_update_pos(usize e_idx, const Bounds& old_bounds, const Bounds& new_bounds_x,
                                               const Bounds& new_bounds_y);

    /// Checks whether the player has reached the goal.
    void check_finish_or_death();

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

    bool finished{false};
    bool dead{false};

    usize index;
};

#endif // GAME_SRC_WORLD_H
