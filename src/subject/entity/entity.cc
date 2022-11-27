#include "entity.h"

using namespace subject::entity;

void Entity::apply_force(const Vec2& f) { this->acceleration += f; }

void Entity::update_physics() {
    this->velocity += this->acceleration;
    // this->pos += this->velocity; /* hanled by world and collision detection */

    this->acceleration = {0, 0};
}

math::Vec2 Entity::project() const { return this->pos + this->velocity + this->acceleration; }
