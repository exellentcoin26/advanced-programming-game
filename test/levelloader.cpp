#include "doctest/doctest.h"

#include "core/utils/levelloader.hpp"
#include "core/utils/types.hpp"

TEST_SUITE("levelloader") {
    TEST_CASE("level_0") {
        const level::LevelInfo level_info = level::load_level_from_file("./test/res/data/level_0.toml");

        const f64 camera_height{0.4};
        const f64 camera_increment{0};
        const math::Vec2 player{1, 1};
        const math::Vec2 goal{8, 1};
        const math::Vec2 size{10, 9};
        const std::vector<math::Vec2> tiles{{0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {7, 8}, {8, 8},
                                            {9, 8}, {0, 7}, {9, 7}, {0, 6}, {9, 6}, {0, 5}, {9, 5}, {0, 4}, {9, 4},
                                            {0, 3}, {9, 3}, {0, 2}, {9, 2}, {0, 1}, {9, 1}, {0, 0}, {1, 0}, {2, 0},
                                            {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}};
        const std::string name{"test_0"};

        CHECK_EQ(camera_height, level_info.camera_height);
        CHECK_EQ(camera_increment, level_info.camera_increment);
        CHECK_EQ(player, level_info.player);
        CHECK_EQ(goal, level_info.goal);
        CHECK_EQ(size, level_info.size);
        CHECK_EQ(tiles, level_info.tiles);
        CHECK_EQ(name, level_info.name);
    }
}
