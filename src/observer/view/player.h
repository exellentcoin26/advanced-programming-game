#pragma once

#ifndef GAME_SRC_OBSERVER_VIEW_PLAYER_H
#define GAME_SRC_OBSERVER_VIEW_PLAYER_H

#include "SFML/Graphics.hpp"

#include "../../subject/entity/entity.h"
#include "../../subject/subject.h"
#include "../../utils/log.h"
#include "../observer.h"

namespace view {

class PlayerViewSFML : public Observer<subject::Subject> {
protected:
    using Window = sf::RenderWindow;
    using Player = subject::entity::Player;

public:
    inline PlayerViewSFML(std::shared_ptr<Window> window) : SubjectRenderer(window) {}
    ~PlayerViewSFML() = default;

    void notify(const subject::Subject& source, ObserverEvent event) override {
        switch (event) {
        case ObserverEvent::PositionChange:
            LOG(Debug) << "Position change event called\n";
            break;
        default:
            break;
        }
    }

protected:
    Window* window;
};

} // namespace view

#endif // GAME_SRC_OBSERVER_VIEW_PLAYER_H
