#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>

// Game Parameters used not only in game.h, but renderer.h, controller.h
constexpr int MAX_RENDER_HIGHSCORES = 9;
constexpr int MAX_PLAYERNAME_LENGTH = 15;

enum GameState : short
{
    TITLE,
    RUN,
    GAMEOVER,
    HIGHSCORES,
    HIGHSCORE_GAINED
};

class HighScore
{
public:
    HighScore(int score, bool recent);
    bool recent; // 今回のハイスコアか否かを判定
    int score;
    std::string name;
    bool operator<(HighScore const& a) const;
};

#endif // GAMESTATE_H