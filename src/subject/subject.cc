#include "subject.h"

#include "../utils/log.h"

using namespace subject;

Bounds::Bounds(const Vec2& left_under, const Vec2& size) : pos(left_under), size(size) {}

bool Bounds::collides(const Bounds& rhs) const {
    bool result = this->get_position().get_x() < rhs.get_position().get_x() + rhs.get_size().get_x() &&
                  this->get_position().get_x() + this->get_size().get_x() > rhs.get_position().get_x() &&
                  this->get_position().get_y() < rhs.get_position().get_y() + rhs.get_size().get_y() &&
                  this->get_position().get_y() + this->get_size().get_y() > rhs.get_position().get_y();

    if (result)
        LOG(Debug) << "Collides\n";

    return result;
}

Subject::Subject(const Vec2& pos, const Bounds& col) : pos(pos), col(col) {}

void Subject::subscribe(std::unique_ptr<Observer<Subject>> observer) { this->observers.push_back(std::move(observer)); }
