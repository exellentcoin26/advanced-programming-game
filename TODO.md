# TODO

- [ ] Decide what cpp version I will be using

- [ ] Setup cpp config

  - [x] Add clang-format config
  - [ ] Add clang-tidy config
  - [ ] Setup make
    - [x] Base config
    - [ ] Build targets
    - [ ] Static logic library
    - [ ] \(Static math library\)
    - [ ] \(Static physics library\)
  - [ ] Setup CI \(CircleCI\)
  - [ ] Structure project
  - [ ] Add a logger
  - [ ] Add stacktraces

- [x] Setup git

  - [x] Add .gitignore

- [ ] Decide on how tests will be done

- [ ] Come up with a name for the game

- [ ] Logic library:

  - [ ] Physics
  - [ ] Math library
  - [ ] Entity design
    - [ ] Decide how to handle collision
    - [ ] Player
      - [ ] Movement controller
  - [ ] Stopwatch
  - [ ] World
  - [ ] Camera (Only positions, not rendering)

- [ ] View
  - [ ] Implement Game class (main program)
  - [ ] Implement state
    - [ ] MenuState
    - [ ] \(OptionsState\)
    - [ ] LevelManagerState
      - [ ] LevelSelector
      - [ ] Level