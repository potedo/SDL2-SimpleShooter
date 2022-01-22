#ifndef ENEMYBULLET_H
#define ENEMYBULLET_H

#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include "object.h"

constexpr int ENEMY_BULLET_SIZE = 16;
const std::string ENEMY_BULLET_IMAGEPATH = "../img/bullet/enemy_bullet.png";

class EnemyBullet : public Object
{
public:
    EnemyBullet(int x, int y, int speed);
};

#endif // ENEMYBULLET_H