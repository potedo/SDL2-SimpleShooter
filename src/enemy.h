#ifndef ENEMY_H
#define ENEMY_H

#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include "object.h"

constexpr int ENEMY_SIZE = 40;
const std::string ENEMY_IMAGEPATH = "../img/enemy/enemy.png";

class Enemy : public Object
{
public:
    Enemy(int x, int y, int speed, float theta, int heightAmplitude);

    // Getter
    int Firecount() { return _fireCount; };

    void IncrementFirecount() { _fireCount++ ; };
    void ResetFirecount() { _fireCount = 0; };

    int BaseY=0;
    float init_theta;
    int heightAmplitude;

private:
    int _fireCount = 0; // これを初期値でランダム化したい
};



#endif // ENEMY_H