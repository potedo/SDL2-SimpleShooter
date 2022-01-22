#include "controller.h"
#include <iostream>
#include "SDL.h"

Controller::Controller(std::size_t screen_width, std::size_t screen_height)
    : screen_width(screen_width), screen_height(screen_height)
{
}

void Controller::HandleInput(bool &running,
                             GameState &state,
                             SoundPlayer &soundPlayer,
                             Player &player, 
                             std::vector<std::unique_ptr<Object>> &bullets,
                             bool highscore_update,
                             std::string &playername,
                             int &bulletTimer) const
{
    SDL_Event e;
    int x_max = screen_width - player.Width() / 2;
    int y_max = screen_height - player.Height() / 2;

    while(SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT) {
            running = false;
        } else if (e.type == SDL_KEYDOWN) {

            auto *states = SDL_GetKeyboardState(NULL);

            if (state == GameState::RUN && player.Health() == Health::ALIVE)
            {
                // 自機移動
                if (states[SDL_SCANCODE_UP])
                {
                    player.y -= player.Speed();
                    if (player.y < player.Height() / 2)
                        player.y = player.Height() / 2;
                }
                if (states[SDL_SCANCODE_DOWN])
                {
                    player.y += player.Speed();
                    if (player.y > y_max)
                        player.y = y_max;
                }
                if (states[SDL_SCANCODE_LEFT])
                {
                    player.x -= player.Speed();
                    if (player.x < player.Width() / 2)
                        player.x = player.Width() / 2;
                }
                if (states[SDL_SCANCODE_RIGHT])
                {
                    player.x += player.Speed();
                    if (player.x > x_max)
                        player.x = x_max;
                }

                // 弾発射：Gameインスタンスの _bullets 変数にスマートポインタをpush_back
                if (states[SDL_SCANCODE_F])
                {
                    bulletTimer++;
                    if (bulletTimer > BULLET_MIN_INTERVAL)
                    {
                        soundPlayer.PlayShotEffect();
                        auto bullet = std::make_unique<Bullet>(player.x, player.y, BULLET_SPEED);
                        bullets.push_back(std::move(bullet));

                        // パワーレベルに応じて、追加の弾を生成
                        if (player.powerLevel >= 2)
                        {
                            auto bullet = std::make_unique<Bullet>(player.x - player.Width() / 4, player.y + player.Height() / 2, BULLET_SPEED);
                            bullets.push_back(std::move(bullet));
                        }
                        if (player.powerLevel >= 3)
                        {
                            auto bullet = std::make_unique<Bullet>(player.x - player.Width() / 4, player.y - player.Height() / 2, BULLET_SPEED);
                            bullets.push_back(std::move(bullet));
                        }
                        if (player.powerLevel >= 4)
                        {
                            auto bullet = std::make_unique<Bullet>(player.x - player.Width() / 2, player.y + player.Height(), BULLET_SPEED);
                            bullets.push_back(std::move(bullet));
                        }
                        if (player.powerLevel >= 5)
                        {
                            auto bullet = std::make_unique<Bullet>(player.x - player.Width() / 2, player.y - player.Height(), BULLET_SPEED);
                            bullets.push_back(std::move(bullet));
                        }
                        // 描画数を抑えるためのカウンタをリセット
                        bulletTimer = 0;
                    }
                }
            }
            else
            {
                if (state == GameState::TITLE)
                {
                    if (states[SDL_SCANCODE_SPACE])
                    {
                        // タイトルでスペースを押したらゲームの進行状態に遷移
                        soundPlayer.PlaySelectEffect();
                        state = GameState::RUN;
                    }
                }

                if(state == GameState::GAMEOVER)
                {
                    if (states[SDL_SCANCODE_SPACE])
                    {
                        // ゲームオーバー画面でスペースを押したとき、ハイスコア更新有無で状態の遷移先を変化
                        soundPlayer.PlaySelectEffect();
                        if (highscore_update)
                        {
                            state = GameState::HIGHSCORE_GAINED;
                        }
                        else
                        {
                            state = GameState::RUN;                            
                        }

                    }

                    if (!highscore_update && states[SDL_SCANCODE_H])
                    {
                        soundPlayer.PlaySelectEffect();
                        state = GameState::HIGHSCORES;
                    }
                }

                if (state == GameState::HIGHSCORES)
                {
                    if (states[SDL_SCANCODE_SPACE])
                    {
                        soundPlayer.PlaySelectEffect();
                        state = GameState::RUN;
                    }
                }

                if (state == GameState::HIGHSCORE_GAINED)
                {
                    // 名前入力のうち、バックスペースはこちらで対処
                    if (states[SDL_SCANCODE_BACKSPACE] && playername.size() > 0)
                    {
                        playername.pop_back();
                    }

                    if (states[SDL_SCANCODE_RETURN])
                    {
                        soundPlayer.PlaySelectEffect();
                        state = GameState::HIGHSCORES;
                    }
                }
            }
        }
        else if (e.type == SDL_TEXTINPUT) // 文字入力はこちら側で処理
        {
            if (state == GameState::HIGHSCORE_GAINED)
            {
                // 名前は15文字まで
                if (playername.size() <= MAX_PLAYERNAME_LENGTH)
                {
                    playername += e.text.text;
                }
            }
        }
    }
}