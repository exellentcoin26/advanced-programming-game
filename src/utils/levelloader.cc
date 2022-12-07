#include "levelloader.h"

#include "tomlplusplus/toml.hpp"

#include "types.h"

using namespace level;

LevelInfo level::load_level_from_file(std::string_view filepath) {
    LevelLoaderExceptionVec exceptions{};

    auto config = toml::parse_file(filepath);

    // ===================
    //    Load from file
    // ===================

    //      Level

    const auto opt_level_map = config["level"]["map"].value<std::string_view>();

    if (!opt_level_map.has_value())
        exceptions.emplace_back("`level` value not set");

    //      Camera

    const u32 camera_height = config["camera"]["heigh"].value_or(2);
    const f64 camera_increment = config["camera"]["increment"].value_or(0);

    if (!exceptions.empty())
        throw exceptions;

    // ===================
    //        Parse
    // ===================

    const std::string level_map{opt_level_map.value()};

    std::stringstream level_map_stream{level_map};

    u32 map_width{0};
    u32 map_height{0};

    auto map_builder = LevelInfo::create(camera_height, camera_increment);

    std::string line;
    while (std::getline(level_map_stream, line, '\n')) {
        if (map_width == 0 && line.size() != 0) {
            // this is first line that is read, thus `map_width` is not yet set
            map_width = line.size();
        } else if (line.size() != map_width) {
            // map is invalid
            exceptions.emplace_back("map size is invalid");

            throw exceptions;
        }

        for (u32 i = 0; i < line.size(); ++i) {
            switch (line.at(i)) {
            case 'x':
                // tile
                map_builder.add_tile(math::Vec2(i, map_height));
                break;
            case 'p':
                // player
                try {
                    map_builder.set_player(math::Vec2(i, map_height));
                } catch (const LevelLoaderExceptionVec& e) {
                    exceptions.insert(exceptions.end(), e.begin(), e.end());
                }
                break;
            case 'g':
                try {
                    map_builder.set_goal(math::Vec2(i, map_height));
                } catch (const LevelLoaderExceptionVec& e) {
                    exceptions.insert(exceptions.end(), e.begin(), e.end());
                }
                // goal
                break;
            default:
                break;
            }
        }

        ++map_height;
    }

    if (!exceptions.empty())
        throw exceptions;

    map_builder.set_size(math::Vec2(map_width, map_height));

    return map_builder.build();
}
