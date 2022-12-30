# include all source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp" "*.hpp")

add_library(core STATIC ${SOURCES})

target_compile_options(core PRIVATE -std=c++17 -Wall -Wextra -Wpedantic)

target_include_directories(core PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/lib/tomlplusplus/include)
