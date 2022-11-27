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

        this->render(pos, cam);

        break;
    }
    default:
        break;
    }
}

void SubjectViewSFML::render(const math::Vec2& pos, const Camera& cam) {
    // project `pos` to camera coordinate system
    const auto& cam_coord_pos = cam.project(pos);

    if (!cam_coord_pos.has_value())
        return;

    // calculate scale factor
    // Note: This uses the fact that the camera's width is [-1, 1].
    const u32 max_window_size = std::max(this->window->getSize().x, this->window->getSize().y);
    const u32 level_x = std::round(max_window_size * (cam.get_width() / std::max(cam.get_width(), cam.get_height())));
    const f64 scale = static_cast<f64>(level_x) / static_cast<f64>(cam.get_width());

    math::Vec2 render_pos = cam_coord_pos.value() * scale;
    render_pos.xn(this->window->getSize().y - render_pos.get_y());

    auto sprite = *this->sprite;
    sprite.setPosition(render_pos.get_x(), render_pos.get_y());
    sprite.scale(20, 20);

    this->window->draw(sprite);

    LOG(Debug) << "Subject rendered\n";
}
