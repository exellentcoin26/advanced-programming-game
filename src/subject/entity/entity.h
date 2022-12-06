#pragma once

#ifndef GAME_SRC_SUBJECT_ENTITY_ENTITY_H
#define GAME_SRC_SUBJECT_ENTITY_ENTITY_H

#include "../subject.h"

namespace subject {

namespace entity {

class Entity : public Subject {
protected:
    const f64 MAX_MOVEMENT_SPEED{0.2f};
    const f64 DRAG_FORCE{0.1f};

public:
    Entity() = default;
    inline Entity(const Vec2& pos, const Bounds& bounds) : Subject(pos, bounds) {}
    virtual ~Entity() = default;

    /// Apllies force to acceleration of entity.
    virtual void apply_force(const Vec2& f);

    /// Update the internal state of the physics of the entity.
    void update_physics();

    /// Returns the new position if the forces and velocity would be applied.
    virtual Vec2 project() const;

    constexpr const Vec2& get_velocity() const { return this->velocity; }
    constexpr Vec2& get_mut_velocity() { return this->velocity; }
    constexpr void set_velocity(const Vec2& vel) { this->velocity = vel; }

protected:
    Vec2 acceleration{};
    Vec2 velocity{};
};

class Player : public Entity {
public:
    inline Player(const Vec2& pos, const Bounds& bounds) : Entity(pos, bounds) {}
    ~Player() = default;

    constexpr Bounds::CollideInfo get_collision_info() const { return this->wall_collider; }
    Bounds::CollideInfo& get_mut_collision_info() { return this->wall_collider; }

private:
    Bounds::CollideInfo wall_collider{};
};

} // namespace entity

} // namespace subject

#endif // GAME_SRC_SUBJECT_ENTITY_ENTITY_H
