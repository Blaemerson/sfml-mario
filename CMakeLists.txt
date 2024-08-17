cmake_minimum_required(VERSION 3.10)
project(SFMLTest)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
# set(SFML_DIR "<sfml root prefix>/lib/cmake/SFML")

find_package(SFML 2.6.1 COMPONENTS window system graphics audio REQUIRED)
add_executable(SFMLTest src/main.cpp src/player.cpp src/eventmanager.cpp src/game.cpp src/window.cpp src/textbox.cpp src/tilemap.cpp)
target_link_libraries(SFMLTest sfml-window sfml-system sfml-graphics sfml-audio)

include_directories(./include)