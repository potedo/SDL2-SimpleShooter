#ifndef DEBRIS_H
#define DEBRIS_H

#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include "object.h"

constexpr int DEBRIS_SIZE = 4;
const std::string DEBRIS_BASE_IMAGEPATH = "../img/debris/debris";

class Debris : public Object
{
public:
    Debris(int x, int y, int speed);
};

#endif // DEBRIS_H