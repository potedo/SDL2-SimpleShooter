#include "game.h"
#include "SDL.h"
#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#include <algorithm>
#include <fstream>
#include <sstream>

Game::Game(Controller &controller, Renderer &renderer, SoundPlayer &soundPlayer)
    : _controller(controller), _renderer(renderer), _soundPlayer(soundPlayer)
{
    _player = Player();
    _player.x = _player.Width()/2;
    _player.y = _controller.ScreenHeight() / 2 - _player.Height() / 2;
    _player.SetTexture(_renderer.loadTexture(_player.ImagePath().c_str()));

    // ハイスコアの初期化 → 外部ファイル読み込み
    std::string line;
    std::string name, score;
    std::ifstream filestream(HIGHSCORES_PATH);

    int cnt = 0;
    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            // 記録されたハイスコアは表示する分しか読み出さない
            if (cnt > MAX_RENDER_HIGHSCORES)
            {
                break;
            }

            std::istringstream linestream(line);
            while (linestream >> name >> score)
            {
                HighScore highscore{std::stoi(score), false};
                highscore.name = name;
                _highscores.push_back(highscore);
            }
            cnt++;
        }
    }

    std::sort(_highscores.rbegin(), _highscores.rend());

    _highScore = _highscores[0].score; // 書き込まれたデータがソートされている前提
}

// -------------------------
//   ゲーム進行メイン関数
// -------------------------

void Game::Run(std::size_t target_frame_duration)
{
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    bool running = true;
    _state = GameState::TITLE;

    while (running)
    {
        if (_state == GameState::TITLE) // タイトル画面の処理
        {
            RunTitle(running, target_frame_duration);
        }
        else if (_state == GameState::RUN) // ゲーム進行中の処理
        {
            RunMain(running, target_frame_duration);
        }
        else if (_state == GameState::GAMEOVER) // ゲームオーバー時の処理
        {
            RunGameOver(running, target_frame_duration);
        }
        else if (_state == GameState::HIGHSCORES) // ハイスコア画面遷移時の処理
        {
            RunHighScores(running, target_frame_duration);
        }
        else if (_state == GameState::HIGHSCORE_GAINED) // ハイスコア更新時画面の処理
        {
            RunHighScoreGained(running, target_frame_duration);
        }
    }

    // 抜けた後、ハイスコアの外部ファイルへの書き込み処理を行う
    WriteHighScores();
}

// -------------------------
//   各状態に応じたゲーム進行関数
// -------------------------
void Game::RunTitle(bool &running, std::size_t target_frame_duration)
{
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    frame_start = SDL_GetTicks();

    _controller.HandleInput(running,
                            _state,
                            _soundPlayer,
                            _player,
                            _bullets,
                            false,
                            _playername,
                            _bulletTimer);
    UpdateTitle();
    _renderer.RenderTitle(_debris);

    frame_end = SDL_GetTicks();
    frame_duration = frame_end - frame_start;
    if (frame_duration < target_frame_duration)
    {
        SDL_Delay(target_frame_duration - frame_duration);
    }
}

void Game::RunMain(bool &running, std::size_t target_frame_duration)
{
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;

    // ゲーム進行中はBGM再生
    if (Mix_PlayingMusic() == 0)
    {
        _soundPlayer.PlayBGM();
    }
    _frame_count++;

    frame_start = SDL_GetTicks();

    _controller.HandleInput(running,
                            _state,
                            _soundPlayer,
                            _player,
                            _bullets,
                            false,
                            _playername,
                            _bulletTimer);
    UpdateMain();

    _renderer.RenderMain(_player,
                         _debris,
                         _bullets,
                         _enemies,
                         _enemyBullets,
                         _explosions,
                         _pods,
                         _powerPods,
                         _score,
                         _highScore,
                         _level);

    frame_end = SDL_GetTicks();
    frame_duration = frame_end - frame_start;

    if (frame_duration < target_frame_duration)
    {
        SDL_Delay(target_frame_duration - frame_duration); 
    }

    // 自機が被弾した場合は、ゲームオーバー状態に遷移
    if (_player.Health() == Health::DEAD)
    {
        // ゲームオーバーに伴い、ハイスコアテーブルを更新
        AddHighScore();
        _state = GameState::GAMEOVER;
    }
}

void Game::RunGameOver(bool &running, std::size_t target_frame_duration)
{
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    // BGM再生中なら、停止
    if (Mix_PlayingMusic())
    {
        Mix_HaltMusic();
        _soundPlayer.FreeBGM();
        _soundPlayer.LoadBGM();
    }

    frame_start = SDL_GetTicks();

    bool highscore_update = _score > _highscores[MAX_RENDER_HIGHSCORES].score;

    _controller.HandleInput(running,
                            _state,
                            _soundPlayer,
                            _player,
                            _bullets,
                            highscore_update,
                            _playername,
                            _bulletTimer);
    UpdateGameOver();

    _renderer.RenderGameOver(_debris,
                             _bullets,
                             _enemies,
                             _enemyBullets,
                             _explosions,
                             _pods,
                             _powerPods,
                             _score,
                             _highScore,
                             highscore_update,
                             _level);

    frame_end = SDL_GetTicks();
    frame_duration = frame_end - frame_start;
    if (frame_duration < target_frame_duration)
    {
        SDL_Delay(target_frame_duration - frame_duration);
    }

    // ハイスコア更新でなく かつ Space key が押された後の処理 → ゲームの初期化(各種変数のリセット)
    if (_state == GameState::RUN)
    {
        Reset();
    }
}

void Game::RunHighScores(bool &running, std::size_t target_frame_duration)
{
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    frame_start = SDL_GetTicks();

    _controller.HandleInput(running,
                            _state,
                            _soundPlayer,
                            _player,
                            _bullets,
                            false,
                            _playername,
                            _bulletTimer);
    UpdateHighScores();
    _renderer.RenderHighScores(_debris, _highscores);

    frame_end = SDL_GetTicks();
    frame_duration = frame_end - frame_start;
    if (frame_duration < target_frame_duration)
    {
        SDL_Delay(target_frame_duration - frame_duration);
    }

    // Space key が押された後の処理 → ゲームの初期化(各種変数のリセット)
    if (_state == GameState::RUN)
    {
        for (int i = 0; i < _highscores.size(); i++)
        {
            _highscores[i].recent = false;
        }
        Reset();
    }
}

void Game::RunHighScoreGained(bool &running, std::size_t target_frame_duration)
{
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    frame_start = SDL_GetTicks();

    _controller.HandleInput(running,
                            _state,
                            _soundPlayer,
                            _player,
                            _bullets,
                            true,
                            _playername,
                            _bulletTimer);
    UpdateHeighScoreGained();
    _renderer.RenderHighScoreGained(_debris, _playername);

    frame_end = SDL_GetTicks();
    frame_duration = frame_end - frame_start;
    if (frame_duration < target_frame_duration)
    {
        SDL_Delay(target_frame_duration - frame_duration);
    }

    // Enterが押された直後
    if (_state == GameState::HIGHSCORES)
    {
        for (int i = 0; i < _highscores.size(); i++)
        {
            if (_highscores[i].recent) // 今回のスコアだった場合
            {
                // 対応ハイスコアのインスタンスに、入力した名前をセッティング
                _highscores[i].name = _playername;
                _playername = ANONYMOUS; // リセット
            }
        }
    }
}

// -------------------------
//   オブジェクト生成用関数
// -------------------------
void Game::GenerateDebris()
{
    // 乱数生成
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(20, _controller.ScreenWidth());
    int x;
    int y;
    int speed;

    // 常に画面上にはMaxStars個の隕石が背景として描画されるものとする
    while (_debris.size() < MaxDebris)
    {
        // 生成位置・移動速度ともにランダム生成
        x = distr(eng);
        y = distr(eng) % _controller.ScreenHeight();
        speed = distr(eng) % MaxDebrisSpeed;
        if (speed < MinDebrisSpeed) speed = MinDebrisSpeed;

        // 1個の隕石を生成
        auto debris = std::make_unique<Debris>(x, y, speed);
        debris->SetTexture(_renderer.loadTexture(debris->ImagePath().c_str()));
        _debris.push_back(std::move(debris));
    }
}

void Game::GenerateEnemies()
{
    // タイマーを用意し、一定時間間隔で生成
    _enemySpawnTimer++;

    int screen_width = _controller.ScreenWidth();
    int screen_height = _controller.ScreenHeight();
    int enemy_y = 0;
    float enemy_theta = 0.0;

    // 敵の生成: 一定時間が経過したら生成
    if (_enemySpawnTimer >= _EnemySpawnFrame)
    {
        // 乱数生成
        std::random_device rd;
        std::mt19937 eng(rd());
        std::uniform_int_distribution<> distr(ENEMY_SIZE, screen_height - ENEMY_SIZE);

        // 敵の生成数はレベルで変化
        for (int i = 0; i < _NumEnemyGenerate; i++)
        {
            // 配置する横位置はランダム。波の初期位相もランダム値とする。
            enemy_y = distr(eng);
            enemy_theta = 3.14 * (float(distr(eng)) / float(screen_height - ENEMY_SIZE));

            // ランダムに生成した横位置・初期位相でランダムに敵を生成
            GenerateEnemy(screen_width - ENEMY_SIZE / 2, enemy_y, enemy_theta);
        }

        _enemySpawnTimer = 0; // 敵生成カウンタをリセット
    }
}

void Game::GenerateEnemyBullets()
{

    for (int i=0; i < _enemies.size(); i++)
    {
        // ダウンキャストで処理(生ポインタ取得 → 所有権はunique_ptrのまま)
        Enemy &enemy(*dynamic_cast<Enemy*>(_enemies[i].get()) );

        // 敵弾生成のためのカウンタを増加
        enemy.IncrementFirecount();

        if (enemy.Firecount() >= _EnemyFireInterval)
        {
            // 敵弾の生成
            GenerateEnemyBullet(enemy.x, enemy.y);            
            enemy.ResetFirecount(); // 敵弾生成のカウンタリセット
        }
    }
}

void Game::GenerateEnemy(int x, int y, float theta)
{
    auto enemy = std::make_unique<Enemy>(x, y, _EnemySpeed, theta, _EnemyHeightAmplitude);
    enemy->SetTexture(_renderer.loadTexture(enemy->ImagePath().c_str()));
    _enemies.push_back(std::move(enemy));
}

void Game::GenerateEnemyBullet(int x, int y)
{
    auto enemyBullet = std::make_unique<EnemyBullet>(x, y, _EnemyBulletSpeed);
    enemyBullet->SetTexture(_renderer.loadTexture(enemyBullet->ImagePath().c_str()));
    _enemyBullets.push_back(std::move(enemyBullet));
}

void Game::GeneratePlayerExplosion()
{
    auto explosion = std::make_unique<Explosion>(_player.x, _player.y, ExplosionSpeedOfPlayer);
    explosion->SetTexture(_renderer.loadTexture(explosion->ImagePath().c_str()));
    _explosions.push_back(std::move(explosion));
}

void Game::GenerateExplosion(int x, int y)
{
    auto explosion = std::make_unique<Explosion>(x, y, ExplosionSpeedOfEnemy);
    explosion->SetTexture(_renderer.loadTexture(explosion->ImagePath().c_str()));
    _explosions.push_back(std::move(explosion));
}


void Game::GeneratePod(int x, int y)
{
    _podGenerateCount = DefaultPodGenerateCount;
    auto pod = std::make_unique<Pod>(x, y, PodSpeed);
    pod->SetTexture(_renderer.loadTexture(pod->ImagePath().c_str()));
    _pods.push_back(std::move(pod));
}

void Game::GeneratePowerPod(int x, int y)
{
    _powerPodGenerateCount = DefaultPowerPodGenerateCount;
    _podGenerateCount = DefaultPodGenerateCount; // パワーポッドが生成されたとき、スコアポッドのカウントリセット
    auto power_pod = std::make_unique<PowerPod>(x, y, PodSpeed);
    power_pod->SetTexture(_renderer.loadTexture(power_pod->ImagePath().c_str()));
    _powerPods.push_back(std::move(power_pod));
}

// -------------------------
//       状態更新用関数
// -------------------------
void Game::UpdateTitle()
{
    UpdateDebris();
}

void Game::UpdateMain()
{
    UpdateDebris();
    UpdateBullets();
    UpdateEnemies();
    UpdateEnemyBullets();
    UpdateExplosions();
    UpdatePods();
    CollisionDetection();
    UpdateLevel();
}

void Game::UpdateGameOver()
{
    UpdateDebris();
    UpdateExplosions();
}

void Game::UpdateHighScores()
{
    UpdateDebris();
}

void Game::UpdateHeighScoreGained()
{
    UpdateDebris();
}

void Game::UpdateDebris()
{
    // 隕石の生成
    GenerateDebris();

    if (_debris.size() > 0)
    {
        for (int i = 0; i < _debris.size(); i++)
        {
            // 隕石を移動
            _debris[i]->x -= _debris[i]->Speed();
            // 画面外になったら隕石の生存状態を無効に
            if (_debris[i]->x < 0) _debris[i]->Killed();
            // Health が DEAD になった隕石は削除
            if (_debris[i]->Health() == Health::DEAD) _debris.erase(_debris.begin() + i);
        }
    }
}

void Game::UpdateBullets()
{
    int screen_width = _controller.ScreenWidth();
    int screen_height = _controller.ScreenHeight();

    int max_x = screen_width;

    if (_bullets.size() > 0)
    {
        for (int i = 0; i < _bullets.size(); i++)
        {
            // 弾を移動
            _bullets[i]->x += _bullets[i]->Speed();
            // 画面外になったら弾の生存状態を無効に
            if (_bullets[i]->x > max_x) _bullets[i]->Killed();
            // Health が DEAD になった弾丸は削除
            if (_bullets[i]->Health() == Health::DEAD ) _bullets.erase(_bullets.begin()+i);
        }
    }
}

void Game::UpdateEnemies()
{
    // 敵の生成
    GenerateEnemies();

    // 敵の状態更新
    for (int i=0; i < _enemies.size(); i++)
    {
        // ダウンキャストで処理(生ポインタ取得 → 所有権はunique_ptrのまま)
        Enemy &enemy(*dynamic_cast<Enemy *>(_enemies[i].get()));

        // 敵を移動
        enemy.x -= enemy.Speed();
        enemy.y = enemy.BaseY + enemy.heightAmplitude * std::sin(3.14 * float(_frame_count) / EnemyWaveCycleFrames + enemy.init_theta);
        enemy.y = enemy.y % _controller.ScreenHeight();

        // 生存判定
        if (enemy.x < 0) enemy.Killed();
        if (enemy.Health() == Health::DEAD)
        {
            _enemies.erase(_enemies.begin() + i); // 元のベクトル側の要素を破棄
        };
    }
}

void Game::UpdateEnemyBullets()
{
    // 敵弾の生成
    GenerateEnemyBullets();

    for (int i = 0; i < _enemyBullets.size(); i++)
    {
        // 弾を移動
        _enemyBullets[i]->x -= _enemyBullets[i]->Speed();
        // 画面外になったら弾の生存状態を無効に
        if (_enemyBullets[i]->x < 0) _enemyBullets[i]->Killed();
        // Health が DEAD になった弾丸は削除
        if (_enemyBullets[i]->Health() == Health::DEAD) _enemyBullets.erase(_enemyBullets.begin() + i);
    }
}

void Game::UpdateExplosions()
{
    // 爆発の状態更新
    for (int i = 0; i < _explosions.size(); i++)
    {
        // ダウンキャストで処理(生ポインタ取得 → 所有権はunique_ptrのまま)
        Explosion &explosion(*dynamic_cast<Explosion *>(_explosions[i].get()));
        // 爆発は数Frame(ExplosionDuration)生存するので、生存時間をカウントアップ → 一定期間経過した爆発の状態変更
        explosion.IncrementLifeCount(); 
        if (explosion.LifeCount() > ExplosionDuration) explosion.Killed();

        // 爆発のサイズ増加
        explosion.SetSize(explosion.LifeCount()*explosion.Speed());
        // Health が DEAD になった爆発は削除
        if (explosion.Health() == Health::DEAD) _explosions.erase(_explosions.begin() + i);
    }
}

void Game::UpdatePods()
{
    // スコアポッドの状態更新
    for (int i = 0; i < _pods.size(); i++)
    {
        _pods[i]->x -= _pods[i]->Speed();
        if (_pods[i]->x < 0) _pods[i]->Killed();
        if (_pods[i]->Health() == Health::DEAD) _pods.erase(_pods.begin() + i);
    }
    // パワーポッドの状態更新
    for (int i = 0; i < _powerPods.size(); i++)
    {
        _powerPods[i]->x -= _powerPods[i]->Speed();
        if (_powerPods[i]->x < 0) _powerPods[i]->Killed();
        if (_powerPods[i]->Health() == Health::DEAD) _powerPods.erase(_powerPods.begin() + i);
    }    
}

void Game::UpdateLevel()
{
    // レベルアップの瞬間判定用の変数
    int before_level = _level;

    // スコアが一定値を超えるとレベルアップ
    for (int i = 0; i < LEVELS.size(); i++)
    {
        if (SCORES_PER_LEVEL[i] <= _score &&
            _score < SCORES_PER_LEVEL[i + 1])
        {
            _level = LEVELS[i];
            _EnemySpawnFrame = DefaultEnemySpawnFrame - i * AccelEnemySpawnFramePerLevel;

            _EnemySpeed = DefaultEnemySpeed + AccelEnemySpeedPerLevel * i;
            _NumEnemyGenerate = DefaultNumEnemyGenerate + i;
            _EnemyBulletSpeed = DefaultEnemyBulletSpeed + AccelEnemyBulletSpeedPerLevel * i;

            _EnemyFireInterval = DefaultEnemyFireInterval - AccelEnemyFireIntervalPerLevel * i;

            _EnemyHeightAmplitude = BaseHeightAmplitudePerLevel * i;
        }
    }

    // レベルアップした瞬間のみ効果音を再生
    if (before_level != _level)
    {
        _soundPlayer.PlayLevelUpEffect();
    }
}

void Game::Reset()
{
    // 自機初期位置設定
    _player.x = _player.Width() / 2;
    _player.y = _controller.ScreenHeight() / 2 - _player.Height() / 2;
    _player.powerLevel = 1;
    _player.Revive();;

    // ゲーム情報クリア
    _bullets.clear();
    _enemies.clear();
    _enemyBullets.clear();
    _explosions.clear();
    _pods.clear();
    _powerPods.clear();
    _podGenerateCount = DefaultPodGenerateCount;
    _powerPodGenerateCount = DefaultPowerPodGenerateCount;
    _score = 0;
    _frame_count = 0;

    // レベルのリセット
    _EnemySpawnFrame = DefaultEnemySpawnFrame;
    _enemySpawnTimer = InitialEnemySpawnFrame;
    _NumEnemyGenerate = DefaultNumEnemyGenerate;
    _EnemySpeed = DefaultEnemySpeed;
    _EnemyFireInterval = DefaultEnemyFireInterval;
    _EnemyBulletSpeed = DefaultEnemyBulletSpeed;
    _EnemyHeightAmplitude = DefaultEnemyHeightAmplitude;
    _level = 1;
}

// -------------------------
//       衝突判定用関数
// -------------------------
void Game::CollisionDetection()
{
    // 自機と敵のの衝突判定
    CollisionDetectionPlayer2Enemies();

    // 敵の弾と自機の衝突判定
    CollisionDetectionPlayer2EnemyBullets();

    // 敵の衝突判定
    CollisionDetectionBullets2Enemies();

    // podとの接触判定 → 接触していればスコアを10加算・特定の音を再生
    CollisionDetectionPlayer2Pods();

    // Power podとの接触判定 → 接触していればパワーレベル上昇・特定の音を再生
    CollisionDetectionPlayer2PowerPods();
}

void Game::CollisionDetectionPlayer2Enemies()
{
    for (int i = 0; i < _enemies.size(); i++)
    {
        if (PlayerCollide2Object(_enemies[i].get()))
        {
            // Playerの状態変更
            _player.Killed();

            // Playerの爆発を生成
            GeneratePlayerExplosion();

            // 爆発音の再生
            _soundPlayer.PlayExplodeEffect();
        }
    }
}

void Game::CollisionDetectionPlayer2EnemyBullets()
{
    for (int i = 0; i < _enemyBullets.size(); i++)
    {
        if (PlayerCollide2Object(_enemyBullets[i].get()))
        {
            // Playerの状態変更
            _player.Killed();

            // Playerの爆発を生成
            GeneratePlayerExplosion();

            // 爆発音の再生
            _soundPlayer.PlayExplodeEffect();
        }
    }
}

void Game::CollisionDetectionBullets2Enemies()
{
    for (int i = 0; i < _enemies.size(); i++)
    {
        for (int j = 0; j < _bullets.size(); j++)
        {
            // 生存している弾と敵機のみ対象衝突判定の対象とする
            if (((_bullets[j]->Health() == Health::ALIVE) && (_enemies[i]->Health() == Health::ALIVE)))
            {
                if (ObjectCollide2Object(_bullets[j].get(), _enemies[i].get()))
                {
                    // 衝突した弾と敵の状態変更
                    _bullets[j]->Killed();
                    _enemies[i]->Killed();

                    // 敵の存在していた位置に爆発を作成
                    GenerateExplosion(_enemies[i]->x, _enemies[i]->y);

                    // 爆発音の再生
                    _soundPlayer.PlayExplodeEffect();

                    // スコアカウント
                    CountScore();

                    // スコアポッド生成のためのカウンタを減算
                    _podGenerateCount--;
                    _powerPodGenerateCount--;

                    // 一定数敵を倒したとき、スコアポッドを生成 → パワーポッドを優先
                    if (_powerPodGenerateCount <= 0)
                    {
                        GeneratePowerPod(_enemies[i]->x, _enemies[i]->y);
                    }
                    else if (_podGenerateCount <= 0) // 通常はスコアポッドを生成
                    {
                        GeneratePod(_enemies[i]->x, _enemies[i]->y);
                    }
                }
            }
        }

    }
}

void Game::CollisionDetectionPlayer2Pods()
{
    for (int i = 0; i < _pods.size(); i++)
    {
        if (_pods[i]->Health() == Health::ALIVE)
        {
            if (PlayerCollide2Object(_pods[i].get()))
            {
                // スコアを10加算
                _score += PodScore;
                if (_score > _highScore) _highScore = _score;

                // スコアポッド取得の音を再生
                _soundPlayer.PlayPodEffect();
                _pods[i]->Killed();
            }
        }
    }
}

void Game::CollisionDetectionPlayer2PowerPods()
{
    for (int i = 0; i < _powerPods.size(); i++)
    {
        if (_powerPods[i]->Health() == Health::ALIVE)
        {
            if (PlayerCollide2Object(_powerPods[i].get()))
            {
                // プレーヤーのパワーレベルを上昇(レベル5まで)
                if (_player.powerLevel < 5)
                {
                    _player.powerLevel++;
                }
                else // パワーレベル最大の時はスコアを20加算
                {
                    _score += PowerPodScore;
                    if (_score > _highScore) _highScore = _score;
                }

                // ポッド取得の音を再生
                _soundPlayer.PlayPodEffect();
                _powerPods[i]->Killed();
            }
        }
    }
}

bool Game::PlayerCollide2Object(Object* object)
{
    int dx;
    int dy;
    float min_dist, dist;

    dx = object->x - _player.x;
    dy = object->y - _player.y;
    min_dist = object->Radius() + _player.Radius() - 10;
    dist = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    return dist < min_dist;
}

bool Game::ObjectCollide2Object(Object* object1, Object* object2)
{
    int dx;
    int dy;
    float min_dist, dist;

    dx = object1->x - object2->x;
    dy = object1->y - object2->y;
    min_dist = object1->Radius() + object2->Radius();
    dist = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    return dist < min_dist;
}

// -------------------------
//       その他内部関数
// -------------------------

void Game::CountScore()
{
    _score++;
    if (_score > _highScore) _highScore = _score;
}

void Game::AddHighScore()
{
    HighScore tmp_score{_score, true};
    tmp_score.name = ANONYMOUS;
    _highscores.push_back(tmp_score);
    std::sort(_highscores.rbegin(), _highscores.rend());
}

void Game::WriteHighScores()
{
    int score;
    std::string name;
    std::ofstream writing_file;
 
    writing_file.open(HIGHSCORES_PATH, std::ios::out);

    if (writing_file.is_open())
    {
        for (int i=0; i < MAX_RENDER_HIGHSCORES; i++)
        {
            score = _highscores[i].score;
            name = _highscores[i].name;           
            writing_file << name << " " << score << std::endl;
        }

    }
    writing_file.close();
}