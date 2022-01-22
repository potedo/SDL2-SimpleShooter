#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <vector>
#include <string>
#include <memory>
#include "object.h"
#include "debris.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "enemy_bullet.h"
#include "explosion.h"
#include "pod.h"
#include "gamestate.h"

constexpr float BlinkDuration = 20.0; // 文字点滅のための定数
const std::string TTF_PATH = "../ttf/Mplus1Code-Regular.ttf";

class Renderer {
public:
    Renderer(){};
    Renderer(std::size_t screen_width, std::size_t screen_height);
    ~Renderer();

    // ゲーム画面描画関数
    void RenderTitle(std::vector<std::unique_ptr<Object>> &debris);
    void RenderMain(Player &player,
                    std::vector<std::unique_ptr<Object>> &debris,
                    std::vector<std::unique_ptr<Object>> &bullets,
                    std::vector<std::unique_ptr<Object>> &enemies,
                    std::vector<std::unique_ptr<Object>> &enemyBullets,
                    std::vector<std::unique_ptr<Object>> &explosions,
                    std::vector<std::unique_ptr<Object>> &pods,
                    std::vector<std::unique_ptr<Object>> &powerPods,
                    int score,
                    int highScore,
                    int level);
    void RenderGameOver(std::vector<std::unique_ptr<Object>> &debris,
                        std::vector<std::unique_ptr<Object>> &bullets,
                        std::vector<std::unique_ptr<Object>> &enemies,
                        std::vector<std::unique_ptr<Object>> &enemyBullets,
                        std::vector<std::unique_ptr<Object>> &explosions,
                        std::vector<std::unique_ptr<Object>> &pods,
                        std::vector<std::unique_ptr<Object>> &powerPods,
                        int score,
                        int highScore,
                        bool highscore_update,
                        int level);
    void RenderHighScores(std::vector<std::unique_ptr<Object>> &debris, std::vector<HighScore> &highscores);
    void RenderHighScoreGained(std::vector<std::unique_ptr<Object>> &debris, std::string playername);

    SDL_Texture *loadTexture(std::string path);

private:
    // テキスト・オブジェクト描画の共通関数
    void RenderText(std::string text, int dest_x, int dest_y, SDL_Color color, float scale, bool left_align = false);
    void RenderObjects(std::vector<std::unique_ptr<Object>> &objects);

    // 単体のObjectなので別枠
    void RenderPlayer(Player &player);

    // RenderObjects() を内部で活用 (テクスチャ読み込みが唯一異なる点)
    void RenderBullets(std::vector<std::unique_ptr<Object>> &bullets);

    // RenderText()を活用
    void RenderScore(int score, int highScore);
    void RenderLevel(int level);
    void RenderGameOverText(bool highscore_update);

    // 内部で保持するSDL関係のオブジェクトへのポインタ
    SDL_Window *_sdl_window = nullptr;
    SDL_Renderer *_sdl_renderer = nullptr;
    SDL_Surface *_surface = nullptr;
    SDL_Texture *_texture = nullptr;
    TTF_Font *_font = nullptr;

    std::size_t _screenWidth;
    std::size_t _screenHeight;

    // 文字点滅のためのカウンタ
    int _BlinkCount = 0;
};


#endif // RENDERER_H