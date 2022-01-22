#ifndef POD_H
#define POD_H

#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include "object.h"

const std::string POD_IMAGEPATH = "../img/pod/pod.png";
const std::string POWERPOD_IMAGEPATH = "../img/pod/power_pod.png";

class Pod : public Object
{
public:
    Pod(int x, int y, int speed);
};

class PowerPod : public Object
{
public:
    PowerPod(int x, int y, int speed);
};

#endif // POD_H