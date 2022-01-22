#include <random>
#include "debris.h"

Debris::Debris(int x, int y, int speed) : Object(x, y, speed)
{
    // 乱数生成 → 生成する隕石の画像をランダムに変更
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(1, 3);
    int debris_number = distr(eng);

    _image_path = DEBRIS_BASE_IMAGEPATH;
    _image_path += std::to_string(debris_number);
    _image_path += ".png";
    _width = DEBRIS_SIZE;
    _height = DEBRIS_SIZE;
    _radius = _width / 2;
    _health = Health::ALIVE;
}