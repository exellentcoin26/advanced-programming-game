#pragma once

#ifndef GAME_SRC_SUBJECT_SUBJECT_H
#define GAME_SRC_SUBJECT_SUBJECT_H

#include "../camera.h"
#include "../math/vec.h"
#include "../observer/observer.h"

#include <memory>
#include <vector>

template <typename T>
class Observer;

namespace subject {

class Bounds {
private:
    using Vec2 = math::Vec2;

public:
public:
    Bounds() = default;
    Bounds(const Vec2& left_under, const Vec2& size);
    ~Bounds() = default;

    bool collides(const Bounds& other) const;

    inline Vec2 get_position() const { return this->pos; }
    inline Vec2 get_size() const { return this->size; }

private:
    /// Position of the left under corner
    const Vec2 pos{};
    const Vec2 size{};
};

class Subject {
protected:
    using Vec2 = math::Vec2;

public:
    Subject() = default;
    Subject(const Vec2& pos, const Bounds& col);
    virtual ~Subject() = default;

    inline Vec2 get_position() const { return this->pos; }
    inline void set_position(const Vec2& pos) { this->pos = pos; }

    /// Updates the intern state of the subject
    virtual void update();

    /// Returns the absolute coordinate bounds of the subject
    inline Bounds get_abs_bounds() const { return {this->col.get_position() + this->pos, this->col.get_size()}; }
    inline Bounds get_rel_bounds() const { return this->col; }

    /// Adds observer to observer list
    void subscribe(std::unique_ptr<Observer<Subject>> observer);

    /// Attaches the given camera to the subject.
    inline void attach_camera(std::shared_ptr<Camera> camera) { this->camera = camera; }
    /// Returns reference to Camera
    inline const Camera& get_camera() const { return *this->camera; }

protected:
    /// normalized position between (width; height)[-1, 1; -1, h]
    Vec2 pos{};
    Bounds col{};

    /// list of observers
    std::vector<std::unique_ptr<Observer<Subject>>> observers{};

    std::shared_ptr<Camera> camera;
};

} // namespace subject

#endif // GAME_SRC_SUBJECT_SUBJECT_H
