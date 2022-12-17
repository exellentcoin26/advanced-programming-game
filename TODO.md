# TODO

- [x] Decide what cpp version I will be using

- [ ] Setup cpp config

  - [x] Add clang-format config
  - [ ] Add clang-tidy config
  - [ ] Setup make
    - [x] Base config
    - [ ] Build targets
      - [x] Base
      - [ ] Expanded
    - [ ] Static logic library
    - [ ] \(Static math library\)
    - [ ] \(Static physics library\)
    - [x] Add header file dependency checking
  - [x] Setup CI \(CircleCI\)
  - [x] Structure project
  - [x] Add a logger
  - [x] Add stacktraces

- [x] Setup git

  - [x] Add .gitignore

- [x] Decide on how tests will be done

- [ ] Come up with a name for the game

- [ ] Logic library:

  - [ ] Physics
    - [ ] Player jump mechanics
        - [ ] Extended bounding box collision detection
  - [x] Math library
  - [x] Entity design
    - [x] Decide how to handle collision
    - [x] Player
      - [x] Movement controller
  - [x] Stopwatch
  - [x] World
  - [x] Camera (Only positions, not rendering)

- [ ] View
  - [x] Implement Game class (main program)
  - [x] Implement state
    - [ ] MenuState
    - [ ] \(OptionsState\)
    - [x] GameState
      - [ ] LevelSelector
      - [x] Level
