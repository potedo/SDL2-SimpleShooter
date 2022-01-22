#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include "object.h"

const std::string EXPLOSION_IMAGEPATH = "../img/enemy/explosion.png";

class Explosion : public Object
{
public:
    Explosion(int x, int y, int speed);

    // Getter
    int LifeCount() { return _lifeCount; };

    // Setter
    void SetSize(int size);

    void IncrementLifeCount() { _lifeCount++; };

private:
    int _lifeCount = 0;
};

#endif // EXPLOSION_H