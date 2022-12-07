#pragma once

#ifndef GAME_SRC_CAMERA_H
#define GAME_SRC_CAMERA_H

#include "math/vec.h"
#include "utils/types.h"

#include <optional>

class Camera {
public:
    constexpr Camera(f64 height, f64 offset, f64 increment = 0.0)
        : height(height), offset(offset), increment(increment) {}
    ~Camera() = default;

    /// Updates the internal state of the camera, like incrementing the offset from the ground, when this is enabled.
    constexpr void update() { this->offset += this->increment; }

    /// Updates the internal state of the camera so that the player_pos is within 85% of the camera height.
    constexpr void track_player(const math::Vec2& player_pos) {
        const f64 max_height = this->offset + this->height * this->PLAYER_MAX_POS;
        if (player_pos.get_y() > max_height) {
            const f64 increment = player_pos.get_y() - max_height;
            this->offset += increment;
        }
    }

    /// Returns wether `pos` is lower than the current view of the camera.
    constexpr bool is_out_of_view_bottom(const math::Vec2& pos) const { return pos.get_y() < this->offset; }

    /// Returns wether `pos` is within the camera view (only bottom and top, not sides).
    inline bool is_out_of_view(const math::Vec2& pos) const {
        return pos.get_y() + 1 < this->offset || pos.get_y() + 1 > this->offset + this->height;
    }

    /// Projects the `pos` to camera coordinate space.
    inline std::optional<math::Vec2> project(const math::Vec2& pos) const {
        if (this->is_out_of_view(pos))
            return std::nullopt;

        return pos + math::Vec2{1.0, 1.0} - math::Vec2{0.0, this->offset};
    }

    inline f64 get_width() const { return this->width; }
    inline f64 get_height() const { return this->height; }

private:
    /// max percentage the player position can reach before the camera follows the player.
    const f64 PLAYER_MAX_POS{0.85};

    /// height of the camera view
    const f64 height{};
    /// width of the camera view (hardcoded for now to [-1, 1])
    const f64 width{2};

    /// offset of the camera view from coordinate (0, 0)
    f64 offset{};
    /// amount the offset increments with every update
    const f64 increment{};
};

#endif // GAME_SRC_CAMERA_H
