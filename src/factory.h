#pragma once

#ifndef GAME_SRC_FACTORY_H
#define GAME_SRC_FACTORY_H

#include "SFML/Graphics.hpp"

#include "subject/factory.h"

#include <unordered_map>

class SFMLSubjectFactory : public SubjectFactory {
private:
    using Window = sf::RenderWindow;
    using Sprite = sf::Sprite;

private:
    /// Enum of different drawable types (used for looking up sprites).
    enum class SubjectType { Player, Tile, Goal };

public:
    SFMLSubjectFactory(std::shared_ptr<Window> window);
    ~SFMLSubjectFactory() = default;

    /// Construct subject with attached SFML specific view and return pointer.
    Subject* create_subject(std::shared_ptr<Camera> cam, const Vec2& pos, const Bounds& bounds) const override;

    /// Construct entity with attached SFML specific view and return pointer.
    Entity* create_entity(std::shared_ptr<Camera> cam, const Vec2& pos, const Bounds& bounds) const override;

    /// Construct player with attached SFML specific view and return pointer.
    Player* create_player(std::shared_ptr<Camera> cam, const Vec2& pos, const Bounds& bounds) const override;

    /// Construct goal with attached SFML specific view and return pointer.
    Goal* create_goal(std::shared_ptr<Camera> cam, const Vec2& pos, const Bounds& bounds) const override;

    /// Construct tile with attached SFML specific view and return pointer.
    Tile* create_tile(std::shared_ptr<Camera> cam, const Vec2& pos, const Bounds& bounds) const override;

private:
    std::shared_ptr<Window> window{};
    std::unique_ptr<sf::Texture> spritesheet{};
    std::unordered_map<SubjectType, std::shared_ptr<Sprite>> sprites{};
};

#endif // GAME_SRC_FACTORY_H
