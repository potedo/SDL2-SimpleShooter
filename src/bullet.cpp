#include "bullet.h"

Bullet::Bullet(int x, int y, int speed) : Object(x, y, speed)
{
    _image_path = BULLET_IMAGEPATH;
    _width = BULLET_WIDTH;
    _height = BULLET_HEIGHT;
    _radius = _width/2;
    _health = Health::ALIVE;
}
