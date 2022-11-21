#pragma once

#ifndef GAME_SRC_OBSERVER_OBSERVER_H
#define GAME_SRC_OBSERVER_OBSERVER_H

enum class ObserverEvent { PositionChange };

/// Abstract observer class
template <typename T>
class Observer {
public:
    virtual ~Observer() = default;

    /// Notify the observer of its task.
    virtual void notify(const T& source, ObserverEvent event) = 0;

protected:
    Observer() = default;
};

#endif // GAME_SRC_OBSERVER_OBSERVER_H
