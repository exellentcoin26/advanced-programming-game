#include "factory.h"

#include "observer/view/subject.h"
#include "utils/log.h"
#include "utils/types.h"

#include <stdexcept>

SFMLSubjectFactory::SFMLSubjectFactory(std::shared_ptr<Window> window) : window(window) {
    // instanciate assets
    const sf::Vector2i player_texture_size{64, 64};
    const sf::Vector2i tile_texture_size{64, 64};

    const sf::Vector2i player_sprite_size{1, 1};
    const sf::Vector2i tile_sprite_size{2, 2};

    sf::FileInputStream spritesheet_stream;
    if (!spritesheet_stream.open("assets/spritesheet.png")) {
        LOG(Error) << "could not load spritesheet\n";
        throw std::runtime_error("failed to load assets");
    }

    this->spritesheet = std::make_unique<sf::Texture>();
    this->spritesheet->loadFromStream(spritesheet_stream);

    auto player_sprite = std::make_shared<sf::Sprite>();
    auto tile_sprite = std::make_shared<sf::Sprite>();

    player_sprite->setTexture(*this->spritesheet);
    tile_sprite->setTexture(*this->spritesheet);
    player_sprite->setTextureRect(sf::IntRect({0, 0}, player_texture_size));
    tile_sprite->setTextureRect(sf::IntRect({67, 0}, tile_texture_size));

    player_sprite->setOrigin(0, player_texture_size.y);
    tile_sprite->setOrigin(0, tile_texture_size.y);

    player_sprite->setScale(static_cast<f64>(player_sprite_size.x) / static_cast<f64>(player_texture_size.x),
                            static_cast<f64>(player_sprite_size.y) / static_cast<f64>(player_texture_size.y));
    tile_sprite->setScale(static_cast<f64>(tile_sprite_size.x) / static_cast<f64>(tile_sprite_size.x),
                          static_cast<f64>(tile_texture_size.y) / static_cast<f64>(tile_texture_size.y));

    this->sprites = {{SubjectType::Player, player_sprite}, {SubjectType::Tile, tile_sprite}};
}

subject::Subject* SFMLSubjectFactory::create_subject(std::shared_ptr<Camera> cam, const Vec2& pos,
                                                     const Bounds& bounds) const {
    return nullptr;
}

subject::entity::Entity* SFMLSubjectFactory::create_entity(std::shared_ptr<Camera> cam, const Vec2& pos,
                                                           const Bounds& bounds) const {
    return nullptr;
}

subject::entity::Player* SFMLSubjectFactory::create_player(std::shared_ptr<Camera> cam, const Vec2& pos,
                                                           const Bounds& bounds) const {
    Player* player = new Player{pos, bounds};
    auto observer = std::unique_ptr<Observer<Subject>>(
        new view::SubjectViewSFML(this->window, this->sprites.at(SubjectType::Player)));

    player->subscribe(std::move(observer));
    player->attach_camera(cam);

    return player;
}
