#include "explosion.h"

Explosion::Explosion(int x, int y, int speed)
    : Object(x, y, speed)
{
    _image_path = EXPLOSION_IMAGEPATH;
    _health = Health::ALIVE;
}

void Explosion::SetSize(int size)
{
    _width = size;
    _height = size;
}