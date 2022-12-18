#include "resource_manager.h"

#include "utils/log.h"

#include <filesystem>

std::shared_ptr<ResourceManager> ResourceManager::instance = 0;

void ResourceManager::load_levels_from_dir(std::string_view dir) {
    level::LevelLoaderExceptionVec exceptions{};

    // check that `dir` exists and is a directory
    if (!std::filesystem::is_directory(dir))
        throw level::LevelLoaderException("`dir` is not a valid direcotry");

    for (const auto& dir_entry : std::filesystem::directory_iterator(dir)) {
        if (dir_entry.path().extension() != ".toml") {
            LOG(Warn) << "Non `toml` file in level directory!\n";
            continue;
        }

        try {
            this->levels.push_back({level::load_level_from_file(dir_entry.path().string()), false});
        } catch (const level::LevelLoaderExceptionVec& e) {
            exceptions.insert(exceptions.end(), e.begin(), e.end());
        }
    }
}
