#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <iostream>
#include <string>

const std::string BGM_PATH = "../sound/BGM/bgm.mp3";
const std::string SHOT_EFFECT_PATH = "../sound/effect/shot.wav";
const std::string EXPLODE_EFFECT_PATH = "../sound/effect/explode.wav";
const std::string POD_EFFECT_PATH = "../sound/effect/pod.wav";
const std::string SELECT_EFFECT_PATH = "../sound/effect/select.wav";
const std::string LEVELUP_EFFECT_PATH = "../sound/effect/Level_Up.wav";

class SoundPlayer
{
public:
    SoundPlayer();
    ~SoundPlayer();

    void PlayBGM();
    void PlayShotEffect();
    void PlayExplodeEffect();
    void PlayPodEffect();
    void PlaySelectEffect();
    void PlayLevelUpEffect();
    void FreeBGM();
    void LoadBGM();

private:
    // BGMを保持
    Mix_Music* _bgm = nullptr;
    // 効果音を保持
    Mix_Chunk* _shot_effect = nullptr;
    Mix_Chunk* _explode_effect = nullptr;
    Mix_Chunk* _pod_effect = nullptr;
    Mix_Chunk* _select_effect = nullptr;
    Mix_Chunk* _levelup_effect = nullptr;
};

#endif // SOUNDPLAYER_H