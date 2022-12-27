#pragma once

#ifndef GAME_SRC_SUBJECT_ENTITY_ENTITY_H
#define GAME_SRC_SUBJECT_ENTITY_ENTITY_H

#include "../subject.hpp"

namespace subject {

namespace entity {

struct CollideInfo {
    /// Collision at the top of the bound.
    bool up;
    /// Collision at the bottom of the bound.
    bool down;
    /// Collision at the left of the bound.
    bool left;
    /// Collision at the right of the bound.
    bool right;

    /// Collides at left or right and bottom.
    inline bool can_wall_jump() const { return (this->left || this->right) && !this->down; }

    /// Returns whether the player is grounded.
    inline bool can_jump() const { return this->down; }
};

class Entity : public Subject {
protected:
    const f64 MAX_MOVEMENT_SPEED{1.5f};
    const f64 DRAG_FORCE{9.0f};

public:
    Entity() = default;
    inline Entity(const Vec2& pos, const Bounds& bounds) : Subject(pos, bounds) {}
    virtual ~Entity() = default;

    /// Apllies force (applied every frame) to acceleration of entity by depending on delta time.
    virtual void apply_force(const Vec2& f);

    /// Applies impulse (applied once in a single frame) to acceleration of entity by ignoring delta time.
    virtual void apply_impulse(const Vec2& i);

    /// Update the internal state of the physics of the entity.
    void update_physics();

    /// Returns the new position if the forces and velocity would be applied.
    virtual Vec2 project() const;

    inline const Vec2& get_velocity() const { return this->velocity; }
    inline Vec2& get_mut_velocity() { return this->velocity; }
    inline void set_velocity(const Vec2& vel) { this->velocity = vel; }

protected:
    Vec2 acceleration{};
    Vec2 velocity{};

private:
    /// Helper variable so that `project` can be called after `update_physics`.
    Vec2 old_acceleration{};
};

class Player : public Entity {
private:
public:
    inline Player(const Vec2& pos, const Bounds& bounds) : Entity(pos, bounds) {}
    ~Player() = default;

    /// Checks if the player can jump after colliding with this object.
    void check_jump_collision(const std::vector<Bounds>& others);

    const CollideInfo& get_jump_collider() const { return this->jump_collider; }

private:
    CollideInfo jump_collider{};
};

} // namespace entity

} // namespace subject

#endif // GAME_SRC_SUBJECT_ENTITY_ENTITY_H
