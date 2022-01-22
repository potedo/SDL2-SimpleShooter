#include "player.h"
#include "bullet.h"

Player::Player()
{
    _image_path = PLAYER_IMAGEPATH;
    x = 0;
    y = 0;
    _speed = PLAYER_SPEED;
    _width = PLAYER_WIDTH;
    _height = PLAYER_HEIGHT;
    _health = Health::ALIVE;
    _radius = _width/2;
}
