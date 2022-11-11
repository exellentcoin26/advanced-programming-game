#include "game.h"

Game::Game(u32 width, u32 height, const std::string& title, u32 fps)
    : width(width), height(height), fps(fps), vsync(true) {
    this->window = std::unique_ptr<Window>(this->create_window(width, height, title));
}

Game::Game(u32 width, u32 height, const std::string& title) : width(width), height(height) {
    this->window = std::unique_ptr<Window>(this->create_window(width, height, title));
}

sf::RenderWindow* Game::create_window(u32 width, u32 height, const std::string& title) const {
    return new Window(sf::VideoMode(width, height), title);
}

void Game::start() {
    log(Debug) << "Main game loop started!\n";

    bool quit = false;
    std::shared_ptr<StopWatch> watch = StopWatch::get_instance();

    const f64 ns = (this->vsync) ? static_cast<f64>(1e9) / static_cast<f64>(this->fps) : 1.0;
    f64 delta{0};

    u64 frames{0};

    TimePoint last_time = SystemClock::now();
    TimePoint time = SystemClock::now();

    while (!quit) {
        TimePoint now = SystemClock::now();
        delta += static_cast<f64>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_time).count()) / ns;
        last_time = now;

        if (delta >= 1) {
            ++frames;
            --delta;

            watch->stop();
            // update and render
            watch->reset_and_start();

            // log information and reset time
            if (std::chrono::duration_cast<std::chrono::milliseconds>(SystemClock::now() - time).count() >= 1000) {
                log(Debug) << "fps: " << frames << '\n';

                time = SystemClock::now();
                frames = 0;
            }
        }
    }
}
