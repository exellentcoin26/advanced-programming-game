#include "core/utils/log.hpp"
#include "game.hpp"

#include <iostream>

int main() {
    auto log_instance = utils::Logger::get_instance();
    log_instance->config(utils::Logger::LogLevel::Debug, std::cout);

    Game game = Game::create_game(Game::WindowStyle::Windowed, 1280, 720, "GAME", true).set_fps(60).build();
    game.start();

    return 0;
}
