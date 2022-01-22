#include "object.h"

Object::Object(int x, int y, int speed) : x(x), y(y), _speed(speed)
{
}

Object::Object(int x, int y, int width, int height, int speed) : x(x), y(y), _width(width), _height(height), _speed(speed)
{
}