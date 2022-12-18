#pragma once

#ifndef GAME_SRC_OBSERVER_VIEW_SUBJECT_H
#define GAME_SRC_OBSERVER_VIEW_SUBJECT_H

#include "SFML/Graphics.hpp"

#include "../observer.h"
#include "math/vec.h"
#include "subject/entity/entity.h"
#include "subject/subject.h"
#include "utils/log.h"

namespace view {

class SubjectViewSFML : public Observer<subject::Subject> {
private:
    using Sprite = sf::Sprite;

protected:
    using Window = sf::RenderWindow;
    using Player = subject::entity::Player;

public:
    SubjectViewSFML(std::shared_ptr<Window> window, std::shared_ptr<Sprite> sprite);
    ~SubjectViewSFML() = default;

    void notify(const subject::Subject& source, ObserverEvent event) override;

    void render(const math::Vec2& pos, const Camera& cam, const math::Vec2& size);

private:
    std::shared_ptr<Window> window{};
    std::shared_ptr<Sprite> sprite{};
};

} // namespace view

#endif // GAME_SRC_OBSERVER_VIEW_SUBJECT_H
