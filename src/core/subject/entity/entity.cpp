#include "entity.hpp"

#include "core/utils/stopwatch.hpp"

using namespace subject::entity;

void Entity::apply_force(const Vec2& f) { this->acceleration += f; }

void Entity::apply_impulse(const Vec2& f) {
    auto watch = utils::StopWatch::get_instance();
    this->acceleration += f / watch->get_delta_time();
}

void Entity::update_physics() {
    auto watch = utils::StopWatch::get_instance();

    // add some drag force
    // (source)[https://forum.unity.com/threads/physics-drag-formula.252406]
    if (this->velocity.get_x() != 0 && this->acceleration.get_x() == 0)
        this->velocity.ny(this->velocity.get_x() * (1 - DRAG_FORCE * watch->get_delta_time()));

    this->velocity += this->acceleration * watch->get_delta_time();
    // this->pos += this->velocity; /* handled by world and collision detection */

    // limit velocity
    this->velocity.ny((this->velocity.get_x() > 0) ? std::min(this->velocity.get_x(), this->MAX_MOVEMENT_SPEED)
                                                   : std::max(this->velocity.get_x(), -this->MAX_MOVEMENT_SPEED));
    // this->velocity.xn(std::min(this->velocity.get_y(), this->MAX_MOVEMENT_SPEED));

    this->old_acceleration = this->acceleration;
    this->acceleration = {0, 0};
}

math::Vec2 Entity::project() const {
    auto watch = utils::StopWatch::get_instance();
    return this->pos + (this->velocity * watch->get_delta_time()) +
           (this->old_acceleration * std::pow(watch->get_delta_time(), 2) * 0.5);
}

void Player::check_jump_collision(const std::vector<Bounds>& others) {
    // check bottom and sides using extended bound colliders, for both sides and the bottom

    // amount to extend the collider with proportional to the current size
    const f64 extend_amount_factor{0.1};

    const f64 extend_amount{extend_amount_factor * this->get_rel_bounds().get_size().get_x()};

    const Bounds side_bounds(
        {this->get_abs_bounds().get_position().get_x() - extend_amount, this->get_abs_bounds().get_position().get_y()},
        {this->get_rel_bounds().get_size().get_x() + extend_amount * 2, this->get_rel_bounds().get_size().get_y()});

    const Bounds bottom_bounds(
        {this->get_abs_bounds().get_position().get_x(), this->get_abs_bounds().get_position().get_y() - extend_amount},
        {this->get_rel_bounds().get_size().get_x(), this->get_rel_bounds().get_size().get_y() + extend_amount});

    CollideInfo info{};

    for (const auto& other : others) {
        const bool side_collides = side_bounds.collides(other);
        const bool bottom_collides = bottom_bounds.collides(other);

        if (side_collides && this->get_abs_bounds().get_position().get_x() < other.get_position().get_x())
            info.right = true;
        else if (side_collides)
            info.left = true;

        if (bottom_collides)
            info.down = true;
    }

    this->jump_collider = info;
}
