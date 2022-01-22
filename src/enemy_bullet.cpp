#include "enemy_bullet.h"

EnemyBullet::EnemyBullet(int x, int y, int speed) : Object(x, y, speed)
{
    _image_path = ENEMY_BULLET_IMAGEPATH;
    _width = ENEMY_BULLET_SIZE;
    _height = ENEMY_BULLET_SIZE;
    _radius = _width / 2;
    _health = Health::ALIVE;
}