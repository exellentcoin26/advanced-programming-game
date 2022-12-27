# =====================
#    Project Config
# =====================

cmake_minimum_required(VERSION 3.20)
project(MeatBoy)

# set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wpedantic")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# ====================
#      Libraries
# ====================

add_subdirectory(lib/sfml)

add_subdirectory(src/core)

# ====================
#   Project Targets
# ====================

# search all source files in top level
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp")
# exclude all files in `core` directory
list(FILTER SOURCES EXCLUDE REGEX "src/core/*")

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "test/*.cpp")

add_executable(meat_boy ${SOURCES})

add_executable(meat_boy-test ${TEST_SOURCES})

# make all files in top level src directory includable
target_include_directories(meat_boy PUBLIC "src" "lib/sfml/include")

target_include_directories(meat_boy-test PUBLIC "src" "lib/doctest/include")

# ====================
#   Library Linking
# ====================

target_link_libraries(meat_boy core sfml-graphics sfml-window sfml-system)

target_link_libraries(meat_boy-test core)
