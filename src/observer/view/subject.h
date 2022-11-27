#pragma once

#ifndef GAME_SRC_OBSERVER_VIEW_SUBJECT_H
#define GAME_SRC_OBSERVER_VIEW_SUBJECT_H

#include "SFML/Graphics.hpp"

#include "../../math/vec.h"
#include "../../subject/entity/entity.h"
#include "../../subject/subject.h"
#include "../../utils/log.h"
#include "../observer.h"

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

    void render(const math::Vec2& pos, const Camera& cam);

private:
    std::shared_ptr<Window> window{};
    std::shared_ptr<Sprite> sprite{};
};

} // namespace view

#endif // GAME_SRC_OBSERVER_VIEW_SUBJECT_H
