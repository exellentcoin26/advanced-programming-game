#include "game.hpp"

#include "controllers/resource_manager.hpp"

Game::GameBuilder::GameBuilder(WindowStyle style, std::pair<u32, u32> size, const std::string& title, bool vsync)
    : style(style), size(std::move(size)), title(title), vsync(vsync) {}

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
      factory(std::make_shared<SFMLSubjectFactory>(this->window)),
      state_manager(std::shared_ptr<StateManager>([&]() -> StateManager* {
          auto resource_manager = ResourceManager::get_instance();
          resource_manager->load_levels_from_dir("assets/levels");

          return StateManager::create_state_manager(StateManager::StateType::MenuState)
              .insert_state(StateManager::StateType::MenuState, nullptr)
              .build();
      }())),
      keyboard(Keyboard()) {
    this->state_manager
        ->insert_state(StateManager::StateType::GameState,
                       new GameState(this->window, this->state_manager, this->factory))
        .insert_state(StateManager::StateType::MenuState, new MenuState(this->window, this->state_manager))
        .insert_state(StateManager::StateType::OptionsState, new OptionsState(this->window, this->state_manager));
}

sf::RenderWindow* Game::create_window(u32 width, u32 height, const std::string& title) const {
    auto style = sf::Style::None;
    switch (this->style) {
    case WindowStyle::Windowed:
        style = sf::Style::Default;
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
            watch->stop();
            watch->reset_and_start();

            ++frames;
            --delta;

            // check for screen close and resize
            sf::Event event;
            while (this->window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    quit = true;
                    break;
                }
                if (event.type == sf::Event::Resized) {
                    sf::FloatRect view(0, 0, event.size.width, event.size.height);
                    window->setView(sf::View(view));
                    this->width = event.size.width;
                    this->height = event.size.height;
                }
            }

            if (quit)
                break;

            this->window->clear();

            this->keyboard.update();
            this->state_manager->update(&this->keyboard);

            this->window->display();

            // log information and reset time
            if (std::chrono::duration_cast<std::chrono::milliseconds>(SystemClock::now() - time).count() >= 1000) {
                LOG(Info) << "fps: " << frames << '\n';

                time = SystemClock::now();
                frames = 0;
            }
        }
    }

    this->window->close();
}
