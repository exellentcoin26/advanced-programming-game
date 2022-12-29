#include "world.hpp"

#include "subject/tile.hpp"
#include "utils/log.hpp"

#include <iostream>

World::World(const level::LevelInfo& level_info, const std::shared_ptr<SubjectFactory>& factory, usize index)
    : index(index) {
    // construct `World` from `level_info`

    // create camera
    this->camera = std::make_shared<Camera>(level_info.camera_height, 0, level_info.camera_increment);

    // A player is about 0.75 percent of a tile
    // calculate factor to restrict the world to be within [-1, 1] on width and [-1, h] on height
    // the level width is set to 2
    const f64 scale = 2.0 / level_info.size.get_x();

    // add player
    this->player = this->subjects.size();
    this->subjects[this->player] = std::unique_ptr<Player>(factory->create_player(
        this->camera, level_info.player * scale - Vec2(1.0, 1.0), Bounds({0, 0}, {scale * 0.75, scale * 0.75})));

    // add player to entities
    this->entities.insert(this->player);

    // add goal
    this->goal = this->subjects.size();
    this->subjects[this->goal] = std::unique_ptr<Goal>(factory->create_goal(
        this->camera, level_info.goal * scale - Vec2(1.0, 1.0), Bounds({0, 0}, {scale * 0.6, scale * 0.6})));

    // add tiles
    for (const Vec2& tile_pos : level_info.tiles) {
        this->subjects[this->subjects.size()] = std::unique_ptr<Tile>(
            factory->create_tile(this->camera, tile_pos * scale - Vec2(1.0, 1.0), Bounds({0, 0}, {scale, scale})));
    }
}

World& World::operator=(World&& world) {
    this->subjects = std::move(world.subjects);
    this->entities = std::move(world.entities);
    this->player = world.player;
    this->goal = world.goal;
    this->dead = world.dead;
    this->finished = world.finished;
    this->camera = world.camera;
    this->index = world.index;

    return *this;
}

void World::update() {
    // LOG(Debug) << "Update\n";

    if (this->finished || this->dead)
        return;

    // update physics for all entities
    for (const usize e_idx : this->entities) {
        Entity* e = static_cast<Entity*>(&*this->subjects.at(e_idx));

        // apply gravity to entity
        e->apply_force(Vec2(0, -GRAVITY));

        // update internal physics state of entity
        e->update_physics();

        // update entity position based on collision
        // construct projeced bounds for move along x and the same for y
        const Vec2 projected_pos = e->project();
        const Vec2 projected_pos_x{projected_pos.get_x(), e->get_position().get_y()};
        const Vec2 projected_pos_y{e->get_position().get_x(), projected_pos.get_y()};

        const Bounds old_bounds = e->get_abs_bounds();
        const Bounds new_bounds_x{projected_pos_x, old_bounds.get_size()};
        const Bounds new_bounds_y{projected_pos_y, old_bounds.get_size()};

        // check collision and update position plus internal state of entity
        this->entity_check_collision_and_update_pos(e_idx, old_bounds, new_bounds_x, new_bounds_y);
    }

    // Update the `ColliderInfo` for the player, by composing a vector of all collidable bounds.
    std::vector<Bounds> bounds{};
    for (const auto& [s_idx, s] : this->subjects) {
        if (this->entities.find(s_idx) != this->entities.end() || s_idx == this->goal)
            // ignore other entities for collision
            continue;

        bounds.push_back(s->get_abs_bounds());
    }

    Player* player = static_cast<Player*>(&*this->subjects.at(this->player));
    player->check_jump_collision(std::move(bounds));

    // force update of all subjects
    for (const auto& [idx, subj] : this->subjects) {
        (void)idx; // ignore unused warning

        subj->update();
    }

    // updated camera and track player
    this->camera->update(this->subjects.at(this->player)->get_position(),
                         this->subjects.at(this->player)->get_rel_bounds().get_size().get_y());

    // check whether the level is completed
    this->check_finish_or_death();
}

void World::move_player(const std::set<Input>& input) {
    if (this->finished || this->dead)
        return;

    Player* player = static_cast<Player*>(&*this->subjects.at(this->player));

    for (auto move : input) {
        switch (move) {
        case Input::Left:
            LOG(Debug) << "Move Left\n";

            player->apply_force(Vec2(-MOVEMENT_FORCE, 0));
            break;

        case Input::Right:
            LOG(Debug) << "Move Right\n";

            player->apply_force(Vec2(MOVEMENT_FORCE, 0));
            break;

        case Input::Jump:
            if (player->get_jump_collider().can_wall_jump()) {
                LOG(Debug) << "Wall Jump!\n";

                player->get_mut_velocity() = {player->get_velocity().get_x(), 0};
                player->apply_impulse({(player->get_jump_collider().left ? JUMP_FORCE : -JUMP_FORCE), JUMP_FORCE});
            } else if (player->get_jump_collider().can_jump()) {
                LOG(Debug) << "Regular Jump!\n";

                player->get_mut_velocity() = {player->get_velocity().get_x(), 0};
                player->apply_impulse(Vec2(0, JUMP_FORCE));
            }
            break;
        }
    }
}

void World::entity_check_collision_and_update_pos(usize e_idx, const Bounds& old_bounds, const Bounds& new_bounds_x,
                                                  const Bounds& new_bounds_y) {
    Vec2 new_pos{new_bounds_x.get_position().get_x(), new_bounds_y.get_position().get_y()};
    Entity* e = static_cast<Entity*>(&*this->subjects.at(e_idx));

    const Bounds new_bounds({new_bounds_x.get_position().get_x(), new_bounds_y.get_position().get_y()},
                            old_bounds.get_size());

    bool collides{false};

    for (const auto& [s_idx, s] : this->subjects) {

        if (this->entities.find(s_idx) != this->entities.end() || s_idx == this->goal || s_idx == e_idx)
            // ignore other entities for collision
            continue;

        // check for x axis
        if (new_bounds_x.collides(s->get_abs_bounds())) {
            // check for wall collision
            new_pos.ny(old_bounds.get_position().get_x());

            // set volocity on x axis to zero
            e->get_mut_velocity().ny(0.0);

            collides = true;
        }

        // check for y axis
        if (new_bounds_y.collides(s->get_abs_bounds())) {
            // check for ground collision
            new_pos.xn(old_bounds.get_position().get_y());

            // set velocity on y axis to zero
            e->get_mut_velocity().xn((e->get_velocity().get_y() > 0) ? -e->get_mut_velocity().get_y() / 3.0 : 0.0);

            collides = true;
        }

        // early-return if there has been a collision at both axes
        if (new_pos == old_bounds.get_position())
            break;
    }

    if (!collides) {
        // check again for all subjects on both axis
        for (const auto& [s_idx, s] : this->subjects) {

            if (this->entities.find(s_idx) != this->entities.end() || s_idx == this->goal || s_idx == e_idx)
                // ignore other entities for collision
                continue;

            if (new_bounds.collides(s->get_abs_bounds())) {
                e->get_mut_velocity() = {0, 0};

                new_pos = old_bounds.get_position();
                break;
            }
        }
    }

    e->set_position(new_pos);
}

void World::check_finish_or_death() {
    const Bounds player_bounds = this->subjects.at(this->player)->get_abs_bounds();
    const Bounds goal_bounds = this->subjects.at(this->goal)->get_abs_bounds();

    if (this->camera->is_out_of_view_bottom(player_bounds.get_position(), player_bounds.get_size().get_y())) {
        this->dead = true;
        std::cout << "Dead!\n";

    } else if (player_bounds.collides(goal_bounds)) {
        this->finished = true;
        std::cout << "Finished!\n";
    }
}
