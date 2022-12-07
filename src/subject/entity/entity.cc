#include "entity.h"

#include "../../utils/stopwatch.h"

using namespace subject::entity;

void Entity::apply_force(const Vec2& f) { this->acceleration += f; }

void Entity::update_physics() {
    auto watch = utils::StopWatch::get_instance();

    this->velocity += this->acceleration * watch->get_delta_time();
    // this->pos += this->velocity; /* hanled by world and collision detection */

    // limit velocity
    this->velocity.ny(std::min(this->velocity.get_x(), this->MAX_MOVEMENT_SPEED));
    this->velocity.xn(std::min(this->velocity.get_y(), this->MAX_MOVEMENT_SPEED));

    this->acceleration = {0, 0};
}

math::Vec2 Entity::project() const {
    auto watch = utils::StopWatch::get_instance();
    return this->pos + this->velocity * watch->get_delta_time();
}
