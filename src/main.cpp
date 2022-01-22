#include <iostream>
#include "game.h"
#include "controller.h"
#include "renderer.h"
#include "sound_player.h"

void Initialize();
void Close();

int main()
{
    constexpr std::size_t kFramePerSecond{60};
    constexpr std::size_t kScreenWidth{1280};
    constexpr std::size_t kScreenHeight{720};

    Initialize();

    Renderer renderer(kScreenWidth, kScreenHeight);
    Controller controller(kScreenWidth, kScreenHeight);
    SoundPlayer soundPlayer;
    Game game(controller, renderer, soundPlayer);

    game.Run(kFramePerSecond);

    Close();

    std::cout << "Game Finished." << std::endl;

    return 0;
}

void Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL Error: " << SDL_GetError() << "\n";
    }
}

void Close()
{
    SDL_Quit();
}