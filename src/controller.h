#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "sound_player.h"
#include "player.h"
#include "bullet.h"
#include "gamestate.h"
#include <vector>
#include <memory>

class Game;

class Controller {
    public:
        Controller(){};
        Controller(std::size_t screen_width, std::size_t screen_height);

        void HandleInput(bool &running,
                         GameState &state,
                         SoundPlayer& soundPlayer,
                         Player &player,
                         std::vector<std::unique_ptr<Object>> &bullets,
                         bool highscore_update,
                         std::string &playername,
                         int &bulletTimer) const;

        std::size_t ScreenWidth() { return screen_width; };
        std::size_t ScreenHeight() { return screen_height; };

    private:
        std::size_t screen_width;
        std::size_t screen_height;
};

#endif // CONTROLLER_H