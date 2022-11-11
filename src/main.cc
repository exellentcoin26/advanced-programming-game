#include "game.h"
#include "utils/log.h"

#include <iostream>

int main() {
    auto log_instance = utils::Logger::get_instance();
    log_instance->config(utils::Logger::LogLevel::Debug, std::cout);

    Game game(1920, 1080, "GAME", 60);

    game.start();

    return 0;
}
