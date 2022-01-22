#ifndef OBJECT_H
#define OBJECT_H

#include "SDL.h"
#include "SDL_image.h"
#include <string>


enum Health
{
    ALIVE,
    DEAD
};

class Object
{
public:
    Object(){};
    virtual ~Object(){};
    Object(int x, int y, int speed);
    Object(int x, int y, int width, int height, int speed);

    // Getter
    SDL_Texture *Texture() { return _texture; };
    std::string ImagePath() { return _image_path; };
    int Speed() { return _speed; };
    int Width() { return _width; };
    int Height() { return _height; };
    int Health() { return _health; };
    int Radius() { return _radius; };

    // Setter
    void SetTexture(SDL_Texture *texture) { _texture = texture; };
    void Killed() { _health = Health::DEAD; };

    // 座標
    int x;
    int y;

protected:
    SDL_Texture *_texture = nullptr;
    std::string _image_path;

    int _width;
    int _height;
    int _health;
    int _radius; // 衝突判定に使用
    int _speed;
};

#endif // OBJECT_H