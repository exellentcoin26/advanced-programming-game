#include "resource_manager.hpp"

#include "core/utils/log.hpp"

#include <filesystem>
#include <map>

std::shared_ptr<ResourceManager> ResourceManager::instance = 0;

void ResourceManager::load_levels_from_dir(std::string_view dir) {
    level::LevelLoaderExceptionVec exceptions{};

    // check that `dir` exists and is a directory
    if (!std::filesystem::is_directory(dir))
        throw level::LevelLoaderException("`dir` is not a valid direcotry");

    // add all entries to entry map, because iterator order is unspecified.
    std::map<std::filesystem::path, std::filesystem::directory_entry> entries;
    for (const auto& dir_entry : std::filesystem::directory_iterator(dir)) {
        if (dir_entry.path().extension() != ".toml") {
            LOG(Warn) << "Non `toml` file in level directory!\n";
            continue;
        }

        entries.insert({dir_entry.path(), dir_entry});
    }

    for (const auto& [_, dir_entry] : entries) {
        if (dir_entry.path().extension() != ".toml") {
            LOG(Warn) << "Non `toml` file in level directory!\n";
            continue;
        }

        try {
            this->levels.push_back({level::load_level_from_file(dir_entry.path().string()), false});
        } catch (const level::LevelLoaderExceptionVec& e) {
            for (const auto& c : e) {
                LOG(Error) << c.what() << '\n';
            }
            exceptions.insert(exceptions.end(), e.begin(), e.end());
        }
    }

    if (!exceptions.empty())
        throw exceptions;
}
