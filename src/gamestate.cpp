#include "gamestate.h"

HighScore::HighScore(int score, bool recent) : score(score), recent(recent)
{
}

bool HighScore::operator<(HighScore const &a) const
{
    return this->score < a.score;
}