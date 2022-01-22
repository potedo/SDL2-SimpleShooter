cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SDL2Test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS}
                    ${SDL2_IMAGE_INCLUDE_DIRS}
                    ${SDL2_MIXER_INCLUDE_DIRS}
                    ${SDL2_TTF_INCLUDE_DIRS}
                    src)

add_executable(SDL_Shooter
    src/main.cpp
    src/game.cpp
    src/gamestate.cpp
    src/controller.cpp
    src/renderer.cpp
    src/object.cpp
    src/debris.cpp
    src/player.cpp
    src/sound_player.cpp
    src/bullet.cpp
    src/enemy.cpp
    src/enemy_bullet.cpp
    src/explosion.cpp
    src/pod.cpp
)

string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(SDL_Shooter
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)
