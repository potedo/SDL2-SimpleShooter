#include "enemy.h"
#include <random>

Enemy::Enemy(int x, int y, int speed, float theta, int heightAmplitude)
    : Object(x, y, speed), BaseY(y), init_theta(theta), heightAmplitude(heightAmplitude)
{
    // 乱数生成
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, 20);

    _fireCount = distr(eng); // 弾の生成タイミングはランダムとする

    _image_path = ENEMY_IMAGEPATH;
    _width = ENEMY_SIZE;
    _height = ENEMY_SIZE;
    _health = Health::ALIVE;
    _radius = _width/2;
}
