#pragma once

#ifndef GAME_SRC_SUBJECT_ENTITY_ENTITY_H
#define GAME_SRC_SUBJECT_ENTITY_ENTITY_H

#include "../subject.h"

namespace subject {

namespace entity {

class Entity : public Subject {
public:
    Entity() = default;
    inline Entity(const Vec2& pos, const Bounds& bounds) : Subject(pos, bounds) {}
    virtual ~Entity() = default;

    /// Apllies force to acceleration of entity.
    virtual void apply_force(const Vec2& f);

    /// Updates the position using the velocity and the applied forces.
    virtual void update();

    /// Returns the new position if the forces and velocity would be applied.
    virtual Vec2 project() const;

    constexpr const Vec2& get_velocity() const { return this->velocity; }
    constexpr void set_velocity(const Vec2& vel) { this->velocity = vel; }

protected:
    Vec2 acceleration{};
    Vec2 velocity{};
};

class Player : public Entity {
public:
    inline Player() : Entity({0, 10}, Bounds({0, 0}, {1, 1})) {}
    ~Player() = default;

    constexpr Bounds::CollideInfo get_collision_info() const { return this->wall_collider; }
    Bounds::CollideInfo& get_mut_collision_info() { return this->wall_collider; }

private:
    Bounds::CollideInfo wall_collider{};
};

} // namespace entity

} // namespace subject

#endif // GAME_SRC_SUBJECT_ENTITY_ENTITY_H
