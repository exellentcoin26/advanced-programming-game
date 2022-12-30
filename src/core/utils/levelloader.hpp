#pragma once

#ifndef GAME_SRC_UTILS_LEVELLOADER_H
#define GAME_SRC_UTILS_LEVELLOADER_H

#include "core/math/vec.hpp"

#include <exception>
#include <optional>
#include <string_view>
#include <vector>

namespace level {

/// Exception thrown when parsing a level file.
class LevelLoaderException : public std::exception {
public:
    inline LevelLoaderException(std::string message) : message(std::move(message)) {}

    inline const char* what() const noexcept override { return message.c_str(); }

private:
    std::string message{};
};

/// Vector of exceptions thrown when parsing level file.
class LevelLoaderExceptionVec : public std::exception, public std::vector<LevelLoaderException> {
public:
    LevelLoaderExceptionVec() = default;
    ~LevelLoaderExceptionVec() = default;

    inline const char* what() const noexcept override { return "failed to parse level, see content for details"; }
};

/// `LevelInfo` struct containing all information of a parsed level file.
struct LevelInfo {
private:
    class LevelInfoBuilder {
    public:
        inline LevelInfoBuilder(std::string name, u32 camera_height, f64 camera_increment)
            : name(std::move(name)), camera_height(camera_height), camera_increment(camera_increment) {}

        inline void set_size(const math::Vec2& size) {
            if (this->size.has_value())
                throw LevelLoaderException("DEV: cannot set size twice");
            this->size = size;
        }

        inline void add_tile(const math::Vec2& tile) { this->tiles.push_back(tile); }

        inline void set_player(const math::Vec2& player) {
            if (this->player.has_value())
                throw LevelLoaderException("cannot set player twice in level map");
            this->player = player;
        }

        inline void set_goal(const math::Vec2& goal) {
            if (this->goal.has_value())
                throw LevelLoaderException("cannot set goal twice in level map");
            this->goal = goal;
        }

        inline LevelInfo build() const {
            LevelLoaderExceptionVec exceptions;

            // check that all atributes are set
            if (!this->size.has_value())
                exceptions.emplace_back("DEV: size value is not set");

            if (!this->player.has_value())
                exceptions.emplace_back("player is not set in level map");

            if (!this->goal.has_value())
                exceptions.emplace_back("goal is not set in level map");

            if (!exceptions.empty())
                throw exceptions;

            // invert all locations
            const math::Vec2 goal = math::Vec2(this->goal->get_x(), this->size->get_y() - this->goal->get_y() - 1);
            const math::Vec2 player =
                math::Vec2(this->player->get_x(), this->size->get_y() - this->player->get_y() - 1);

            std::vector<math::Vec2> tiles(this->tiles.size());
            for (u32 i = 0; i < tiles.size(); ++i) {
                tiles[i] = math::Vec2(this->tiles[i].get_x(), this->size->get_y() - this->tiles[i].get_y() - 1);
            }

            // recalculate camera_height and camera_increment to be proportional to width of level (only n *tiles*
            // should be visible)
            const f64 scale = 2.0 / this->size->get_x();
            const f64 camera_height = this->camera_height * scale;
            const f64 camera_increment = this->camera_increment * scale;

            return {std::move(this->name), this->size.value(), tiles, player, goal, camera_height, camera_increment};
        }

    private:
        const std::string name{};
        std::optional<math::Vec2> size{};
        std::vector<math::Vec2> tiles{};
        std::optional<math::Vec2> player{};
        std::optional<math::Vec2> goal{};
        const u32 camera_height{};
        const f64 camera_increment{};
    };

public:
    /// Creates a new instance of the `LevelInfoBuilder` class.
    inline static LevelInfoBuilder create(std::string name, u32 camera_height, f64 camera_increment) {
        return {std::move(name), camera_height, camera_increment};
    }

public:
    /// Name of the level.
    const std::string name{};
    /// Size of the level axes.
    const math::Vec2 size{};
    /// Vector of tile coordinates in the level.
    /// Note: Coordinates start from `{0, 0}` in the left-bottom corner.
    const std::vector<math::Vec2> tiles{};
    /// Coordinates of the player.
    /// Note: Coordinates start from `{0, 0}` in the left-bottom corner.
    const math::Vec2 player{};
    /// Coordinates of the goal.
    /// Note: Coordinates start from `{0, 0}` in the left-bottom corner.
    const math::Vec2 goal{};
    /// Amount of tiles (height) seen by the camera.
    const f64 camera_height{};
    /// Amount the camera offset increments with every second.
    const f64 camera_increment{};
};

/// Parsers a level file into a `LevelInfo` instance.
LevelInfo load_level_from_file(std::string_view filepath);

} // namespace level

#endif // GAME_SRC_UTILS_LEVELLOADER_H
