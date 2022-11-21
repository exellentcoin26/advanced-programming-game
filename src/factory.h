#pragma once

#ifndef GAME_SRC_FACTORY_H
#define GAME_SRC_FACTORY_H

#include "SFML/Graphics.hpp"

#include "subject/factory.h"

class SFMLSubjectFactory : public SubjectFactory {
private:
    using Window = sf::RenderWindow;

public:
    inline SFMLSubjectFactory(Window* window) : window(window) {}
    ~SFMLSubjectFactory() = default;

    /// Construct subject with attached SFML specific view and return pointer.
    Subject* create_subject(const Vec2& pos, const Bounds& bounds) const override;

    /// Construct entity with attached SFML specific view and return pointer.
    Entity* create_entity(const Vec2& pos, const Bounds& bounds) const override;

    /// Construct player with attached SFML specific view and return pointer.
    Player* create_player(const Vec2& pos, const Bounds& bounds) const override;

private:
    Window* window;
};

#endif // GAME_SRC_FACTORY_H
