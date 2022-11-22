#include "game.h"

Game::GameBuilder::GameBuilder(WindowStyle style, std::pair<u32, u32> size, const std::string& title, bool vsync)
    : style(style), size(std::move(size)), title(std::move(title)), vsync(vsync) {}

Game::GameBuilder& Game::GameBuilder::set_fps(u32 fps) {
    if (!this->vsync)
        throw std::runtime_error("cannot set fps when vsync is set to false");

    this->fps = fps;

    return *this;
}

Game Game::GameBuilder::build() const {
    if (this->vsync && !this->fps.has_value())
        throw std::runtime_error("vsync is set to true, but nu fps value given");
    const u32 fps = this->fps.value_or(0);

    return {this->style, this->size.first, this->size.second, this->title, fps, this->vsync};
}

Game::GameBuilder Game::create_game(WindowStyle style, u32 width, u32 height, const std::string& title, bool vsync) {
    return {style, {width, height}, title, vsync};
}

Game::Game(WindowStyle style, u32 width, u32 height, const std::string& title, u32 fps, bool vsync)
    : style(style), width(width), height(height),
      window(std::shared_ptr<Window>(this->create_window(width, height, title))), fps(fps), vsync(vsync),
      state_manager(StateManager::create_state_manager(StateType::GameState)
                        .insert_state(StateType::GameState, new GameState(this->window))
                        .insert_state(StateType::MenuState, new MenuState(this->window))
                        .insert_state(StateType::OptionsState, new OptionsState(this->window))
                        .build()),
      keyboard(Keyboard()) {}

sf::RenderWindow* Game::create_window(u32 width, u32 height, const std::string& title) const {
    auto style = sf::Style::None;
    switch (this->style) {
    case WindowStyle::Windowed:
        style = sf::Style::Close;
        break;
    case WindowStyle::BorderLess:
        style = sf::Style::None;
        break;
    case WindowStyle::FullScreen:
        style = sf::Style::Fullscreen;
        break;
    default:
        throw std::runtime_error("WindowStyle not implemented");
        break;
    }

    return new Window(sf::VideoMode(width, height), title, style);
}

void Game::start() {
    LOG(Debug) << "Main game loop started!\n";

    bool quit = false;
    std::shared_ptr<StopWatch> watch = StopWatch::get_instance();

    const f64 ns = (this->vsync) ? static_cast<f64>(1e9) / static_cast<f64>(this->fps) : 1.0;
    f64 delta{0};

    u64 frames{0};

    TimePoint last_time = SystemClock::now();
    TimePoint time = SystemClock::now();

    while (this->window->isOpen() && !quit) {
        TimePoint now = SystemClock::now();
        delta += static_cast<f64>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_time).count()) / ns;
        last_time = now;

        if (delta >= 1) {
            ++frames;
            --delta;

            // check for screen close
            sf::Event event;
            while (this->window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    quit = true;
                    break;
                }
            }

            if (quit)
                break;

            // clear the screen
            this->window->clear();

            watch->stop();

            // update and render
            this->keyboard.update();
            this->state_manager.update(&keyboard);

            watch->reset_and_start();

            this->window->display();

            // log information and reset time
            if (std::chrono::duration_cast<std::chrono::milliseconds>(SystemClock::now() - time).count() >= 1000) {
                LOG(Debug) << "fps: " << frames << '\n';

                time = SystemClock::now();
                frames = 0;
            }
        }
    }

    this->window->close();
}
