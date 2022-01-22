#include "pod.h"

Pod::Pod(int x, int y, int speed) : Object(x, y, speed)
{
    _image_path = POD_IMAGEPATH;
    _width = 30;
    _height = 30;
    _radius = _width / 2;
    _health = Health::ALIVE;
}

PowerPod::PowerPod(int x, int y, int speed) : Object(x, y, speed)
{
    _image_path = POWERPOD_IMAGEPATH;
    _width = 30;
    _height = 30;
    _radius = _width / 2;
    _health = Health::ALIVE;
}