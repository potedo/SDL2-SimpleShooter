#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include "object.h"

constexpr int PLAYER_WIDTH = 60;
constexpr int PLAYER_HEIGHT = 60;
constexpr int PLAYER_SPEED = 20;
const std::string PLAYER_IMAGEPATH = "../img/player/player.png";

class Player : public Object{
public:
    Player();

    void Revive() { _health = Health::ALIVE; };
    int powerLevel = 1;
};

#endif // PLAYER_H