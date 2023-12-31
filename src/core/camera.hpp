#pragma once

#ifndef GAME_SRC_CAMERA_H
#define GAME_SRC_CAMERA_H

#include "math/vec.hpp"
#include "utils/stopwatch.hpp"
#include "utils/types.hpp"

#include <optional>

/// Camera class representing what can be seen by the window.
class Camera {
public:
    /// Creates a new `Camera` instance.
    inline Camera(f64 height, f64 offset, f64 increment = 0.0) : height(height), offset(offset), increment(increment) {}
    ~Camera() = default;

    /// Updates the internal state of the camera, like incrementing the offset from the ground, when this is enabled or
    /// following the player when no increment is specified.
    inline void update(const math::Vec2& player_pos, f64 player_height) {
        if (this->increment == 0) {
            this->track_player(player_pos, player_height);
            return;
        }

        auto watch = utils::StopWatch::get_instance();
        this->offset += this->increment * watch->get_delta_time();
    }

    /// Updates the internal state of the camera so that the player_pos is within 85% of the camera height.
    inline void track_player(const math::Vec2& player_pos, f64 player_height) {
        const f64 max_height = this->offset + this->height * this->PLAYER_MAX_POS;
        if (player_pos.get_y() + player_height + 1 > max_height) {
            const f64 increment = player_pos.get_y() + player_height + 1 - max_height;
            this->offset += increment;
        }
    }

    /// Returns wether `pos` is lower than the current view of the camera.
    inline bool is_out_of_view_bottom(const math::Vec2& pos, f64 height) const {
        return pos.get_y() + height + 1 < this->offset;
    }

    /// Returns wether `pos` is within the camera view (only bottom and top, not sides).
    inline bool is_out_of_view(const math::Vec2& pos, f64 height) const {
        return pos.get_y() + height + 1 < this->offset || pos.get_y() + 1 > this->offset + this->height;
    }

    /// Projects the `pos` to camera coordinate space.
    inline std::optional<math::Vec2> project(const math::Vec2& pos, f64 height) const {
        if (this->is_out_of_view(pos, height))
            return std::nullopt;

        return pos + math::Vec2{1.0, 1.0} - math::Vec2{0.0, this->offset};
    }

    /// Returns the view width of the `Camera`.
    inline f64 get_width() const { return this->width; }
    /// Returns the view height of the `Camera`.
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
