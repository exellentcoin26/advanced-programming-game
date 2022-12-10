#include "subject.h"

#include "../../utils/log.h"

using namespace view;

SubjectViewSFML::SubjectViewSFML(std::shared_ptr<Window> window, std::shared_ptr<Sprite> sprite)
    : window(window), sprite(sprite) {}

void SubjectViewSFML::notify(const subject::Subject& source, ObserverEvent event) {
    switch (event) {
    case ObserverEvent::PositionChange: {
        LOG(Debug) << "Position change event called\n";

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

    math::Vec2 render_pos = cam_coord_pos.value() * scale;
    render_pos.xn(this->window->getSize().y - render_pos.get_y());

    auto sprite = *this->sprite;
    sprite.setPosition(render_pos.get_x(), render_pos.get_y());
    sprite.setScale(
        sf::Vector2f(size.get_x() / sprite.getTextureRect().width, size.get_y() / sprite.getTextureRect().height));
    sprite.scale(sf::Vector2f(scale, scale));

    this->window->draw(sprite);

    LOG(Debug) << "Subject rendered\n";
}
