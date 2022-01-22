#ifndef BULLET_H
#define BULLET_H

#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include "object.h"

constexpr int BULLET_WIDTH = 20;
constexpr int BULLET_HEIGHT = 16;
constexpr int BULLET_SPEED = 50;
constexpr int BULLET_MIN_INTERVAL = 1;
const std::string BULLET_IMAGEPATH = "../img/bullet/bullet.png";

class Bullet : public Object
{
public:
    Bullet(int x, int y, int speed);
};

#endif // BULLET_H