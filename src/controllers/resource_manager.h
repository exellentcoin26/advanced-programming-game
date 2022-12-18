#ifndef GAME_SRC_CONTROLLER_RESOURCE_MANAGER_H
#define GAME_SRC_CONTROLLER_RESOURCE_MANAGER_H

#include "subject/factory.h"
#include "utils/levelloader.h"

#include <exception>
#include <memory>

/// Singleton class to manage game resources.
class ResourceManager {
public:
    static inline std::shared_ptr<ResourceManager> get_instance() {
        if (!instance)
            instance = std::shared_ptr<ResourceManager>(new ResourceManager());
        return instance;
    }

    ResourceManager(const ResourceManager&) = delete;
    void operator=(const ResourceManager&) = delete;

public:
    ~ResourceManager() = default;

    /// Try to parse all toml files in `dir` as `LevelInfo` objects.
    void load_levels_from_dir(std::string_view dir);

    /// Returns all `{LevelInfo, completed}` pairs.
    inline const std::vector<std::pair<level::LevelInfo, bool>>& get_levels() const { return this->levels; }

    /// Returns `{LevelInfo, completed}` of the current level.
    inline const std::pair<level::LevelInfo, bool>& get_current_level() const {
        return this->levels.at(this->current_level);
    }

    /// Returns the index of the current level.
    inline u32 get_current_level_index() const { return this->current_level; }

    /// Tries to set the current level to the given index. Returns `false` if the index is larger or equal to the
    /// `levels` vector size. Returns `true` if operation succeeded.
    inline bool set_current_level(u32 index) {
        if (index >= this->levels.size())
            return false;

        this->current_level = index;
        return true;
    }

private:
    ResourceManager() = default;

private:
    /// List of `{LevelInfo, completed}` objects used in the game.
    std::vector<std::pair<level::LevelInfo, bool>> levels{};
    u32 current_level{0};

private:
    static std::shared_ptr<ResourceManager> instance;
};

#endif // GAME_SRC_CONTROLLER_RESOURCE_MANAGER_H
