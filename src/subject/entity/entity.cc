#include "entity.h"

#include "utils/log.h"
#include "utils/stopwatch.h"

using namespace subject::entity;

void Entity::apply_force(const Vec2& f) { this->acceleration += f; }

void Entity::update_physics() {
    auto watch = utils::StopWatch::get_instance();

    // add some drag force
    if (this->velocity.get_x() != 0 && this->acceleration.get_x() == 0)
        this->apply_force({DRAG_FORCE * -this->velocity.get_x() * 3, 0});

    this->velocity += this->acceleration * watch->get_delta_time();
    // this->pos += this->velocity; /* hanled by world and collision detection */

    // limit velocity
    this->velocity.ny((this->velocity.get_x() > 0) ? std::min(this->velocity.get_x(), this->MAX_MOVEMENT_SPEED)
                                                   : std::max(this->velocity.get_x(), -this->MAX_MOVEMENT_SPEED));
    // this->velocity.xn(std::min(this->velocity.get_y(), this->MAX_MOVEMENT_SPEED));

    this->acceleration = {0, 0};
}

math::Vec2 Entity::project() const {
    auto watch = utils::StopWatch::get_instance();
    return this->pos + this->velocity * watch->get_delta_time();
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
