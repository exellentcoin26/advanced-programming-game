#include "subject.hpp"

#include "core/utils/log.hpp"

using namespace view;

SubjectViewSFML::SubjectViewSFML(std::shared_ptr<Window> window, std::shared_ptr<Sprite> sprite)
    : window(window), sprite(sprite) {}

void SubjectViewSFML::notify(const subject::Subject& source, ObserverEvent event) {
    switch (event) {
    case ObserverEvent::PositionChange: {
        // LOG(Debug) << "Position change event called\n";

        // render player using the necessary information
        const Camera& cam = source.get_camera();
        const math::Vec2& pos = source.get_position();
        const math::Vec2& size = source.get_rel_bounds().get_size();

        this->render(pos, cam, size);

        break;
    }
    default:
        break;
    }
}

void SubjectViewSFML::render(const math::Vec2& pos, const Camera& cam, const math::Vec2& size) {
    // project `pos` to camera coordinate system
    const auto cam_coord_pos = cam.project(pos, size.get_y());

    if (!cam_coord_pos.has_value())
        return;

    // calculate scale factor
    // Note: Only keeps the camera height within view, does not use level- or window width.
    const f64 scale = this->window->getSize().y / cam.get_height();

    // calculate offset from bottom left corner to left corner of the level.
    // Note: This is only for looks. When parts of the level are out of view, the camera views the leftmost part of the
    // map, without following the player.
    // TODO: Make camera follow player on the x axis.
    const f64 level_width = cam.get_width() * scale;
    const f64 remaining_window_size = this->window->getSize().x - level_width;

    const f64 offset = (remaining_window_size < 0) ? 0 : remaining_window_size / 2;

    math::Vec2 render_pos = cam_coord_pos.value() * scale;
    render_pos.xn(this->window->getSize().y - render_pos.get_y()).ny(render_pos.get_x() + offset);

    auto sprite = *this->sprite;
    sprite.setPosition(render_pos.get_x(), render_pos.get_y());
    sprite.setScale(
        sf::Vector2f(size.get_x() / sprite.getTextureRect().width, size.get_y() / sprite.getTextureRect().height));
    sprite.scale(sf::Vector2f(scale, scale));

    this->window->draw(sprite);

    // LOG(Debug) << "Subject rendered\n";
}
