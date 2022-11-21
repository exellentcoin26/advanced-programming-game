#include "factory.h"

#include "observer/view/player.h"

subject::Subject* SFMLSubjectFactory::create_subject(const Vec2& pos, const Bounds& bounds) const { return nullptr; }

subject::entity::Entity* SFMLSubjectFactory::create_entity(const Vec2& pos, const Bounds& bounds) const {
    return nullptr;
}

subject::entity::Player* SFMLSubjectFactory::create_player(const Vec2& pos, const Bounds& bounds) const {
    Player* player = new Player{pos, bounds};
    auto observer = std::unique_ptr<Observer<Subject>>(new view::PlayerViewSFML(this->window));

    player->subscribe(std::move(observer));

    return player;
}
