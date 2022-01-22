#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(std::size_t screen_width,
                   std::size_t screen_height)
    : _screenWidth(screen_width),
      _screenHeight(screen_height) {

    _sdl_window = SDL_CreateWindow("SDL Shooter",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   screen_width,
                                   screen_height,
                                   SDL_WINDOW_SHOWN);

    if (nullptr == _sdl_window)
    {
        std::cerr << "Window could not be created.\n";
        std::cerr << "SDL Error: " << SDL_GetError() << "\n";
    }

    _sdl_renderer = SDL_CreateRenderer(_sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (nullptr == _sdl_renderer)
    {
        std::cerr << "Renderer could not be created.\n";
        std::cerr << "SDL Error: " << SDL_GetError() << "\n";
    }
    else
    {
        SDL_SetRenderDrawColor(_sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        int imgFlags = IMG_INIT_PNG;
        if ( !( IMG_Init(imgFlags) & imgFlags ) )
        {
            std::cerr << "SDL_image could not initialize.\n";
            std::cerr << "SDL_image Error: " << IMG_GetError() << "\n";
        }

        // フォント関係初期化
        if ( TTF_Init() < 0 )
        {
            std::cerr << "TTF could not initialize." << std::endl;
            std::cerr << TTF_GetError() << std::endl;
        }

        // 30はポイント数。表示したいサイズに合わせて調整 → 変数化？
        _font = TTF_OpenFont(TTF_PATH.c_str(), 30);
        if (nullptr == _font)
        {
            std::cout << "TTF_OpenFont: " << std::endl;
            std::cout << TTF_GetError() << std::endl;
        }
    }
}

Renderer::~Renderer() {
    SDL_DestroyWindow(_sdl_window);
}

// -------------------
//   ゲーム画面描画関数
// -------------------
void Renderer::RenderTitle(std::vector<std::unique_ptr<Object>> &debris)
{
    // スクリーンをクリア
    SDL_SetRenderDrawColor(_sdl_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(_sdl_renderer);

    // 隕石描画
    RenderObjects(debris);

    std::string title = "SDL SHOOTER";
    std::string key_prompt = "PRESS SPACE TO PLAY!";

    // ここから先は、本来良い感じの画像で表示したいところ
    // タイトルの描画
    SDL_Color color{255, 255, 255, 255};
    RenderText(title, int(_screenWidth) / 2, int(_screenHeight) / 5*2, color, 3);

    // press space key to play again の描画
    SDL_Color yellow_blink_color{255, 255, Uint8(255 * float(_BlinkCount) / BlinkDuration), 0};
    RenderText(key_prompt, int(_screenWidth) / 2, int(_screenHeight) / 2 + 80, yellow_blink_color, 1);

    // バッファに描画した絵を画面に表示
    SDL_RenderPresent(_sdl_renderer);

    _BlinkCount++;
    if (_BlinkCount >= BlinkDuration)
    {
        _BlinkCount = 0;
    }
}



void Renderer::RenderMain(Player &player,
                          std::vector<std::unique_ptr<Object>> &debris,
                          std::vector<std::unique_ptr<Object>> &bullets,
                          std::vector<std::unique_ptr<Object>> &enemies,
                          std::vector<std::unique_ptr<Object>> &enemyBullets,
                          std::vector<std::unique_ptr<Object>> &explosions,
                          std::vector<std::unique_ptr<Object>> &pods,
                          std::vector<std::unique_ptr<Object>> &powerPods,
                          int score,
                          int highScore,
                          int level)
{
    // スクリーンをクリア
    SDL_SetRenderDrawColor(_sdl_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(_sdl_renderer);

    // 背景の隕石描画
    RenderObjects(debris);

    // プレイヤーの描画
    RenderPlayer(player);

    // 弾の描画
    RenderBullets(bullets);

    // 敵の描画
    RenderObjects(enemies);

    // 敵の弾の描画
    RenderObjects(enemyBullets);

    // 爆発の描画
    RenderObjects(explosions);

    // スコアポッド・パワーポッドの描画
    RenderObjects(pods);
    RenderObjects(powerPods);

    // スコアの描画
    RenderScore(score, highScore);

    // レベルの描画
    RenderLevel(level);

    // バッファに描画した絵を画面に表示
    SDL_RenderPresent(_sdl_renderer);
}

void Renderer::RenderGameOver(std::vector<std::unique_ptr<Object>> &debris,
                              std::vector<std::unique_ptr<Object>> &bullets,
                              std::vector<std::unique_ptr<Object>> &enemies,
                              std::vector<std::unique_ptr<Object>> &enemyBullets,
                              std::vector<std::unique_ptr<Object>> &explosions,
                              std::vector<std::unique_ptr<Object>> &pods,
                              std::vector<std::unique_ptr<Object>> &powerPods,
                              int score,
                              int highScore,
                              bool highscore_update,
                              int level)
{
    // スクリーンをクリア
    SDL_SetRenderDrawColor(_sdl_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(_sdl_renderer);

    // 背景の隕石描画
    RenderObjects(debris);

    // 弾の描画
    RenderBullets(bullets);

    // 敵の描画
    RenderObjects(enemies);

    // 敵の弾の描画
    RenderObjects(enemyBullets);

    // 爆発の描画
    RenderObjects(explosions);

    // スコアポッド・パワーポッドの描画
    RenderObjects(pods);
    RenderObjects(powerPods);

    // スコアの描画
    RenderScore(score, highScore);

    // レベルの描画
    RenderLevel(level);

    // GameOverの文字列描画
    RenderGameOverText(highscore_update);

    // バッファに描画した絵を画面に表示
    SDL_RenderPresent(_sdl_renderer);
}

void Renderer::RenderHighScores(std::vector<std::unique_ptr<Object>> &debris, std::vector<HighScore> &highscores)
{
    // スクリーンをクリア
    SDL_SetRenderDrawColor(_sdl_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(_sdl_renderer);

    // 隕石描画
    RenderObjects(debris);

    std::string highscores_text = "HIGHSCORES";
    std::string key_prompt = "PRESS SPACE TO PLAY AGAIN!";

    // ハイスコアの描画
    SDL_Color color{255, 255, 255, 255};
    SDL_Color yellow_blink_color{255, 255, Uint8(255 * float(_BlinkCount) / BlinkDuration), 0};
    RenderText(highscores_text, int(_screenWidth) / 2, 70, color, 1.5);

    // ハイスコア描画
    std::string highscore_str;
    int highscore_int;
    for (int i = 0; i < MAX_RENDER_HIGHSCORES; i++)
    {
        highscore_str = "0.                    000";
        highscore_int = highscores[i].score;
        int score_digits = std::to_string(highscore_int).size();
        if (score_digits < 4)
        {
            highscore_str.replace(22 + 3 - score_digits, score_digits, std::to_string(highscore_int));
        }
        else
        {
            highscore_str.replace(22, 3, "999");
        }
        highscore_str.replace(0, 1, std::to_string(i + 1));
        highscore_str.replace(3, highscores[i].name.size(), highscores[i].name);
        if (highscores[i].recent)
        {
            RenderText(highscore_str, int(_screenWidth) / 2, 140 + 40 * i, yellow_blink_color, 1);
        }
        else
        {
            RenderText(highscore_str, int(_screenWidth) / 2, 140 + 40 * i, color, 1);
        }
    }

    // press space key to play! の描画
    RenderText(key_prompt, int(_screenWidth) / 2, int(_screenHeight) / 5 * 4, yellow_blink_color, 1);

    // バッファに描画した絵を画面に表示
    SDL_RenderPresent(_sdl_renderer);

    _BlinkCount++;
    if (_BlinkCount >= BlinkDuration)
    {
        _BlinkCount = 0;
    }
}

void Renderer::RenderHighScoreGained(std::vector<std::unique_ptr<Object>> &debris, std::string playername)
{
    // スクリーンをクリア
    SDL_SetRenderDrawColor(_sdl_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(_sdl_renderer);

    // 隕石描画
    RenderObjects(debris);

    // ハイスコア記録用文字列描画
    std::string congraturation_text = "Congraturations! You've gained A HIGHSCORE!";
    std::string enter_text = "Enter your name below:";
    std::string key_prompt = "PRESS RETURN KEY WHEN FINISHED";

    SDL_Color color{255, 255, 255, 255};
    SDL_Color yellow_blink_color{255, 255, Uint8(255 * float(_BlinkCount) / BlinkDuration), 0};
    SDL_Color input_color{0, 255, 255, 255};

    playername += "_";

    RenderText(congraturation_text, int(_screenWidth) / 2, int(_screenHeight) / 5, color, 1);
    RenderText(enter_text, int(_screenWidth) / 2, int(_screenHeight) / 5 + 40, color, 1);
    RenderText(playername, int(_screenWidth) / 2, int(_screenHeight) / 5 * 2, input_color, 1);
    RenderText(key_prompt, int(_screenWidth) / 2, int(_screenHeight) / 5 * 4, yellow_blink_color, 1);

    // バッファに描画した絵を画面に表示
    SDL_RenderPresent(_sdl_renderer);
    _BlinkCount++;
    if (_BlinkCount >= BlinkDuration)
    {
        _BlinkCount = 0;
    }
}
// ---------------------------------
//    テキスト・オブジェクト描画共通関数
// ---------------------------------
void Renderer::RenderText(std::string text, int dest_x, int dest_y, SDL_Color color, float scale, bool left_align)
{
    _surface = TTF_RenderUTF8_Blended(_font, text.c_str(), color);
    _texture = SDL_CreateTextureFromSurface(_sdl_renderer, _surface);

    int iw, ih;
    SDL_QueryTexture(_texture, NULL, NULL, &iw, &ih);

    SDL_Rect txtRect = (SDL_Rect){0, 0, iw, ih};
    iw *= scale;
    ih *= scale;
    SDL_Rect pasteRect;
    if (left_align)
    {
        pasteRect = (SDL_Rect){dest_x, dest_y - ih / 2, iw, ih};
    }
    else
    {
        pasteRect = (SDL_Rect){dest_x - iw / 2, dest_y - ih / 2, iw, ih};
    }

    SDL_RenderCopy(_sdl_renderer, _texture, &txtRect, &pasteRect);
}

void Renderer::RenderObjects(std::vector<std::unique_ptr<Object>> &objects)
{
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i]->Health() == Health::ALIVE)
        {
            SDL_Rect DestR;
            DestR.w = objects[i]->Width();
            DestR.h = objects[i]->Height();
            DestR.x = objects[i]->x - objects[i]->Width() / 2;
            DestR.y = objects[i]->y - objects[i]->Height() / 2;

            SDL_RenderCopy(_sdl_renderer, objects[i]->Texture(), NULL, &DestR);
        }
    }
}

// ---------------------------------
//    特定オブジェクト描画関数
// ---------------------------------
void Renderer::RenderPlayer(Player &player)
{
    SDL_Rect DestR;
    DestR.w = player.Width();
    DestR.h = player.Height();
    DestR.x = player.x - player.Width()/2;
    DestR.y = player.y - player.Height()/2;

    SDL_RenderCopy(_sdl_renderer, player.Texture(), NULL, &DestR);
}

void Renderer::RenderBullets(std::vector<std::unique_ptr<Object>> &bullets)
{
    for (int i=0; i < bullets.size(); i++)
    {
        // controllerでbulletが生成される関係上、Textureの読み込みはここでやらざるを得ない
        if (nullptr == bullets[i]->Texture())
        {
            bullets[i]->SetTexture(loadTexture(bullets[i]->ImagePath().c_str()));
        }

        RenderObjects(bullets);
    }
}

// ---------------------------------
//          特定文字列描画関数
// ---------------------------------
void Renderer::RenderScore(int score, int highScore)
{
    // --------------------
    //      文字列の作成
    // --------------------
    // スコアは3桁まで表示することにする
    std::string score_str = "SCORE: 000";
    std::string score_int = std::to_string(score);
    int score_digits = score_int.size();
    if (score_digits < 4)
    {
        score_str.replace(7 + 3 - score_digits, score_digits, score_int);
    }
    else
    {
        score_str.replace(7, 3, "999");
    }

    // ハイスコアも3桁まで表示
    std::string highscore_str = "HIGHSCORE: 000";
    std::string highscore_int = std::to_string(highScore);
    int highscore_digits = highscore_int.size();
    if (highscore_digits < 4)
    {
        highscore_str.replace(11 + 3 - highscore_digits, highscore_digits, highscore_int);
    }
    else
    {
        highscore_str.replace(11, 3, "999");
    }

    // -----------------------
    //  描画 (位置はこの数値で固定)
    // -----------------------
    SDL_Color color{255, 255, 255, 255};
    RenderText(score_str, 100, 30, color, 1);    
    RenderText(highscore_str, int(_screenWidth) - 130, 30, color, 1);
}

void Renderer::RenderLevel(int level)
{
    // レベルは2桁まで表示することにする
    std::string level_str = "LEVEL: 00";
    std::string level_int = std::to_string(level);
    int level_digits = level_int.size();
    if (level_digits < 3)
    {
        level_str.replace(7 + 2 - level_digits, level_digits, level_int);
    }
    else
    {
        level_str.replace(7, 2, "99");
    }

    // -----------------------
    //  描画 (位置はこの数値で固定)
    // -----------------------
    SDL_Color color{255, 255, 255, 255};
    RenderText(level_str, int(_screenWidth)/2, 30, color, 1);
    if (level >= 10)
    {
        SDL_Color yellow{255, 255, 0, 255};
        RenderText(level_str, int(_screenWidth) / 2, 30, yellow, 1);
    }
}

void Renderer::RenderGameOverText(bool highscore_update)
{
    std::string gameover = "Game Over";
    std::string key_prompt = "press space key to play again";
    std::string highscore_prompt = "press H key to show HIGHSCORES";

    SDL_Color color{255, 255, 255, 255};
    SDL_Color yellow_blink_color{255, 255, Uint8(255 * float(_BlinkCount) / BlinkDuration), 0};

    if (highscore_update)
    {
        std::string congraturation_text = "You've gained HIGHSCORE!";
        std::string name_key_prompt = "Press Space key to Enter your name.";
        SDL_Color pink_blink_color{255, Uint8(255 * float(_BlinkCount) / BlinkDuration), 255, 0};
        // press space key to Enter your name の描画
        RenderText(congraturation_text, int(_screenWidth) / 2, int(_screenHeight) / 2 + 80, color, 1);
        RenderText(name_key_prompt, int(_screenWidth) / 2, int(_screenHeight) / 2 + 120, pink_blink_color, 1);
    }
    else
    {
        // press space key to play again の描画
        RenderText(key_prompt, int(_screenWidth) / 2, int(_screenHeight) / 2 + 80, yellow_blink_color, 1);

        // press H key to show HIGHSCORES の描画
        RenderText(highscore_prompt, int(_screenWidth) / 2, int(_screenHeight) / 2 + 120, color, 0.75);
    }

    // Game Overの描画
    RenderText(gameover, int(_screenWidth) / 2, int(_screenHeight) / 2, color, 2);

    _BlinkCount++;
    if(_BlinkCount >= BlinkDuration)
    {
        _BlinkCount = 0;
    }
}

// SDL_RendererがTextureのロードに必要なため、Renderer内に定義
SDL_Texture *Renderer::loadTexture(std::string path)
{
    SDL_Texture *newTexture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cerr << "Unable to load image!\n";
        std::cerr << "SDL_image Error: " << IMG_GetError() << "\n";
    }
    else
    {
        // 背景色透過の処理(R255 G0 B255 の色を透過させる設定)
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0, 0xFF) );
        
        newTexture = SDL_CreateTextureFromSurface(_sdl_renderer, loadedSurface);
        if (newTexture == NULL)
        {
            std::cout << "Unbale to create texture from surface! SDL Error: " << SDL_GetError() << std::endl;
        }

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}