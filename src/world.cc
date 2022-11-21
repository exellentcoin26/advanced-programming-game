#include "world.h"

#include "subject/tile.h"
#include "utils/log.h"

World::World()
    : subjects([]() {
          std::map<usize, std::unique_ptr<Subject>> m;
          m[0] = std::make_unique<Player>();
          m[1] = std::make_unique<Goal>();
          m[2] = std::make_unique<subject::Tile>();
          return m;
      }()),
      entities({0}), player(0), goal(1) {}

void World::update() {
    // LOG(Debug) << "Update\n";

    // update physics for all entities
    for (const usize e_idx : this->entities) {
        Entity* e = static_cast<Entity*>(&*this->subjects.at(e_idx));

        // apply gravity to entity
        e->apply_force(Vec2{0, -GRAVITY});

        // update internal state of entity
        e->update();

        // update entity position based on collision
        // construct projeced bounds for move along x and the same for y
        const Vec2 projected_pos = e->project();
        const Vec2 projected_pos_x{projected_pos.get_x(), e->get_position().get_y()};
        const Vec2 projected_pos_y{e->get_position().get_x(), projected_pos.get_y()};

        const Bounds old_bounds = e->get_abs_bounds();
        const Bounds new_bounds_x{projected_pos_x, old_bounds.get_size()};
        const Bounds new_bounds_y{projected_pos_y, old_bounds.get_size()};

        // check collision and update position plus internal state of entity
        this->entity_check_collision(e_idx, old_bounds, new_bounds_x, new_bounds_y);
    }
}

void World::move_player(Input input) {
    Player* player = static_cast<Player*>(&*this->subjects.at(this->player));

    switch (input) {
    case Input::Left:
        LOG(Debug) << "Move Left\n";

        player->apply_force(Vec2{-MOVEMENT_FORCE, 0});
        break;

    case Input::Right:
        LOG(Debug) << "Move Right\n";

        player->apply_force(Vec2{MOVEMENT_FORCE, 0});
        break;

    case Input::Jump:
        if (player->get_collision_info().down) {
            LOG(Debug) << "Jump!\n";

            player->apply_force(Vec2{0, JUMP_FORCE});

        } else if (player->get_collision_info().is_wall_colliding()) {
            LOG(Debug) << "Wall Jump!\n";

            player->apply_force(Vec2{(player->get_collision_info().left) ? JUMP_FORCE : -JUMP_FORCE, JUMP_FORCE});
        }
        break;
    }
}

void World::entity_check_collision(usize e_idx, const Bounds& old_bounds, const Bounds& new_bounds_x,
                                   const Bounds& new_bounds_y) {
    Vec2 new_pos{new_bounds_x.get_position().get_x(), new_bounds_y.get_position().get_y()};
    Entity* e = static_cast<Entity*>(&*this->subjects.at(e_idx));

    for (const auto& [s_idx, s] : this->subjects) {

        if (this->entities.find(s_idx) != this->entities.end())
            // ignore other entities for collision
            continue;

        // check for x axis
        if (new_bounds_x.collides(s->get_abs_bounds())) {
            // check for wall collision
            new_pos.ny(old_bounds.get_position().get_x());
        }

        // check for y axis
        if (new_bounds_y.collides(s->get_abs_bounds())) {
            // check for ground collision
            new_pos.xn(old_bounds.get_position().get_y());
            e->set_velocity({0, 0});
        }

        // early-return if there has been a collision at both axes
        if (new_pos == old_bounds.get_position())
            break;
    }

    e->set_position(new_pos);
}
