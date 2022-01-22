#include "sound_player.h"

SoundPlayer::SoundPlayer()
{
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
    {
        std::cerr << "SDL_mixer could not initialize.\n";
        std::cerr << "SDL_mixer Error: " << Mix_GetError() << "\n";
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        std::cerr << "SDL_mixer could not initialize.\n"; 
        std::cerr << "SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

    _bgm = Mix_LoadMUS(BGM_PATH.c_str());
    _shot_effect = Mix_LoadWAV(SHOT_EFFECT_PATH.c_str());
    _explode_effect = Mix_LoadWAV(EXPLODE_EFFECT_PATH.c_str());
    _pod_effect = Mix_LoadWAV(POD_EFFECT_PATH.c_str());
    _select_effect = Mix_LoadWAV(SELECT_EFFECT_PATH.c_str());
    _levelup_effect = Mix_LoadWAV(LEVELUP_EFFECT_PATH.c_str());
}

SoundPlayer::~SoundPlayer()
{
    Mix_Quit();
}

void SoundPlayer::PlayBGM()
{
    if (Mix_PlayMusic(_bgm, -1) == -1)
    {
        std::cerr << "Mix_PlayMusic: \n";
        std::cerr << Mix_GetError() << std::endl;
    }
}

void SoundPlayer::PlayShotEffect()
{
    Mix_PlayChannel( -1, _shot_effect, 0);
}

void SoundPlayer::PlayExplodeEffect()
{
    Mix_PlayChannel( -1, _explode_effect, 0);
}

void SoundPlayer::PlayPodEffect()
{
    Mix_PlayChannel( -1, _pod_effect, 0);
}

void SoundPlayer::PlaySelectEffect()
{
    Mix_PlayChannel( -1, _select_effect, 0);
}

void SoundPlayer::PlayLevelUpEffect()
{
    Mix_PlayChannel( -1, _levelup_effect, 0);
}

void SoundPlayer::FreeBGM()
{
    Mix_FreeMusic(_bgm);
}

void SoundPlayer::LoadBGM()
{
    _bgm = Mix_LoadMUS(BGM_PATH.c_str());
}
