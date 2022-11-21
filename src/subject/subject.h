#pragma once

#ifndef GAME_SRC_SUBJECT_SUBJECT_H
#define GAME_SRC_SUBJECT_SUBJECT_H

#include "../math/vec.h"

namespace subject {

class Bounds {
private:
    using Vec2 = math::Vec2;

public:
    struct CollideInfo {
        /// Collision at the top of the bound.
        bool up;
        /// Collision at the bottom of the bound.
        bool down;
        /// Collision at the left of the bound.
        bool left;
        /// Collision at the right of the bound.
        bool right;

        /// Collides at any side of the bound.
        constexpr bool collides() const { return this->up || this->down || this->left || this->right; }

        /// Collides at left or right side.
        constexpr bool is_wall_colliding() const { return this->left || this->right; }
    };

public:
    Bounds() = default;
    Bounds(const Vec2& left_under, const Vec2& size);
    ~Bounds() = default;

    bool collides(const Bounds& other) const;

    constexpr Vec2 get_position() const { return this->pos; }
    constexpr Vec2 get_size() const { return this->size; }

private:
    /// Position of the left under corner
    const Vec2 pos{};
    const Vec2 size{};
};

class Subject {
protected:
    using Vec2 = math::Vec2;

public:
    Subject() = default;
    Subject(const Vec2& pos, const Bounds& col);
    virtual ~Subject() = default;

    inline Vec2 get_position() const { return this->pos; }
    inline void set_position(const Vec2& pos) { this->pos = pos; }

    /// Returns the absolute coordinate bounds of the subject
    inline Bounds get_abs_bounds() const { return {this->col.get_position() + this->pos, this->col.get_size()}; }
    constexpr Bounds get_rel_bounds() const { return this->col; }

protected:
    // normalized position between (width; height)[-1, 1; -1, h] of the left under corner
    Vec2 pos{};
    Bounds col{};
};

}; // namespace subject

#endif // GAME_SRC_SUBJECT_SUBJECT_H
