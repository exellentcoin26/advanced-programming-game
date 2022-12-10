#include "factory.h"

#include "observer/view/subject.h"
#include "utils/log.h"
#include "utils/types.h"

#include <stdexcept>

SFMLSubjectFactory::SFMLSubjectFactory(std::shared_ptr<Window> window) : window(window) {
    // instanciate assets
    const sf::Vector2i player_texture_size{64, 64};
    const sf::Vector2i tile_texture_size{64, 64};
    const sf::Vector2i goal_texture_size{64, 64};

    const sf::Vector2i player_sprite_size{1, 1};
    const sf::Vector2i tile_sprite_size{2, 2};
    const sf::Vector2f goal_sprite_size{1.5, 1.5};

    sf::FileInputStream spritesheet_stream;
    if (!spritesheet_stream.open("assets/spritesheet.png")) {
        LOG(Error) << "could not load spritesheet\n";
        throw std::runtime_error("failed to load assets");
    } else {
        LOG(Debug) << "assets loaded correctly\n";
    }

    this->spritesheet = std::make_unique<sf::Texture>();
    this->spritesheet->loadFromStream(spritesheet_stream);

    auto player_sprite = std::make_shared<sf::Sprite>();
    auto tile_sprite = std::make_shared<sf::Sprite>();
    auto goal_sprite = std::make_shared<sf::Sprite>();

    player_sprite->setTexture(*this->spritesheet);
    tile_sprite->setTexture(*this->spritesheet);
    goal_sprite->setTexture(*this->spritesheet);
    player_sprite->setTextureRect(sf::IntRect({0, 0}, player_texture_size));
    tile_sprite->setTextureRect(sf::IntRect({66, 0}, tile_texture_size));
    goal_sprite->setTextureRect(sf::IntRect({132, 0}, goal_texture_size));

    player_sprite->setOrigin(0, player_texture_size.y);
    tile_sprite->setOrigin(0, tile_texture_size.y);
    goal_sprite->setOrigin(0, goal_texture_size.y);

    player_sprite->setScale(static_cast<f64>(player_sprite_size.x) / static_cast<f64>(player_texture_size.x),
                            static_cast<f64>(player_sprite_size.y) / static_cast<f64>(player_texture_size.y));
    tile_sprite->setScale(static_cast<f64>(tile_sprite_size.x) / static_cast<f64>(tile_texture_size.x),
                          static_cast<f64>(tile_sprite_size.y) / static_cast<f64>(tile_texture_size.y));
    goal_sprite->setScale(static_cast<f64>(goal_sprite_size.x) / static_cast<f64>(goal_texture_size.x),
                          static_cast<f64>(goal_sprite_size.y) / static_cast<f64>(goal_texture_size.y));

    this->sprites = {
        {SubjectType::Player, player_sprite}, {SubjectType::Tile, tile_sprite}, {SubjectType::Goal, goal_sprite}};
}

subject::Subject* SFMLSubjectFactory::create_subject(std::shared_ptr<Camera> cam, const Vec2& pos,
                                                     const Bounds& bounds) const {
    // avoid warinings
    (void)cam;
    (void)pos;
    (void)bounds;

    return nullptr;
}

subject::entity::Entity* SFMLSubjectFactory::create_entity(std::shared_ptr<Camera> cam, const Vec2& pos,
                                                           const Bounds& bounds) const {
    // avoid warinings
    (void)cam;
    (void)pos;
    (void)bounds;

    return nullptr;
}

subject::entity::Player* SFMLSubjectFactory::create_player(std::shared_ptr<Camera> cam, const Vec2& pos,
                                                           const Bounds& bounds) const {
    Player* player = new Player(pos, bounds);
    auto observer = std::unique_ptr<Observer<Subject>>(
        new view::SubjectViewSFML(this->window, this->sprites.at(SubjectType::Player)));

    player->subscribe(std::move(observer));
    player->attach_camera(cam);

    return player;
}

subject::Goal* SFMLSubjectFactory::create_goal(std::shared_ptr<Camera> cam, const Vec2& pos,
                                               const Bounds& bounds) const {
    Goal* goal = new Goal(pos, bounds);
    auto observer = std::unique_ptr<Observer<Subject>>(
        new view::SubjectViewSFML(this->window, this->sprites.at(SubjectType::Goal)));

    goal->subscribe(std::move(observer));
    goal->attach_camera(cam);

    return goal;
}

subject::Tile* SFMLSubjectFactory::create_tile(std::shared_ptr<Camera> cam, const Vec2& pos,
                                               const Bounds& bounds) const {
    Tile* tile = new Tile(pos, bounds);
    auto observer = std::unique_ptr<Observer<Subject>>(
        new view::SubjectViewSFML(this->window, this->sprites.at(SubjectType::Tile)));

    tile->subscribe(std::move(observer));
    tile->attach_camera(cam);

    return tile;
}
