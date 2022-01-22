#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "sound_player.h"
#include "debris.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "enemy_bullet.h"
#include "explosion.h"
#include "pod.h"
#include "gamestate.h"
#include <vector>
#include <memory>

// Level Configuration
const std::vector<int> LEVELS{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const std::vector<int> SCORES_PER_LEVEL{0, 15, 40, 70, 100, 140, 200, 250, 300, 350, 999};
constexpr int AccelEnemySpawnFramePerLevel = 8;
constexpr int AccelEnemySpeedPerLevel = 2;
constexpr int AccelEnemyBulletSpeedPerLevel = 3;
constexpr int AccelEnemyFireIntervalPerLevel = 5;
constexpr int BaseHeightAmplitudePerLevel = 30;

// Debris Parameters
constexpr int MaxDebris = 200;
constexpr int MaxDebrisSpeed = 30;
constexpr int MinDebrisSpeed = 1;

// Enemy Parameters
constexpr int DefaultEnemySpawnFrame = 100;
constexpr int InitialEnemySpawnFrame = 90;
constexpr int DefaultNumEnemyGenerate = 2;
constexpr int DefaultEnemySpeed = 1;
constexpr int DefaultEnemyFireInterval = 60;
constexpr int DefaultEnemyBulletSpeed = 10;
constexpr int DefaultEnemyHeightAmplitude = 0;
constexpr int EnemyWaveCycleFrames = 40;

// Pod Generation Parameters
constexpr int PodSpeed = 4;
constexpr int DefaultPodGenerateCount = 5;
constexpr int DefaultPowerPodGenerateCount = 15;
constexpr int PodScore = 10;
constexpr int PowerPodScore = 20;

// Explosion Parameters
constexpr int ExplosionDuration = 5;
constexpr int ExplosionSpeedOfPlayer = 30;
constexpr int ExplosionSpeedOfEnemy = 16;

// Other Parameters
const std::string ANONYMOUS = "ANONYMOUS";
const std::string HIGHSCORES_PATH = "../highscore/highscores.txt";


class Game {
    public:
        Game(){};
        Game(Controller &controller, Renderer &renderer, SoundPlayer &soundPlayer);

        // ゲーム進行メイン関数
        void Run(std::size_t target_frame_duration);

    private:
        // 各状態に応じたゲーム進行関数
        void RunTitle(bool &running, std::size_t target_frame_duration);
        void RunMain(bool &running, std::size_t target_frame_duration);
        void RunGameOver(bool &running, std::size_t target_frame_duration);
        void RunHighScores(bool &running, std::size_t target_frame_duration);
        void RunHighScoreGained(bool &running, std::size_t target_frame_duration);

        // オブジェクト生成用関数
        void GenerateDebris();
        void GenerateEnemies();
        void GenerateEnemyBullets();
        void GeneratePlayerExplosion();
        void GenerateEnemy(int x, int y, float theta);
        void GenerateEnemyBullet(int x, int y);
        void GenerateExplosion(int x, int y);
        void GeneratePod(int x, int y);
        void GeneratePowerPod(int x, int y);

        // ゲーム状態更新用関数
        void UpdateTitle();
        void UpdateMain();
        void UpdateGameOver();
        void UpdateHighScores();
        void UpdateHeighScoreGained();
        void UpdateDebris();
        void UpdateBullets();
        void UpdateEnemies();
        void UpdateEnemyBullets();
        void UpdateExplosions();
        void UpdatePods();
        void UpdateLevel();
        void Reset();

        // 衝突判定用関数: オブジェクト毎に分割
        void CollisionDetection(); // メイン
        void CollisionDetectionPlayer2Enemies();
        void CollisionDetectionPlayer2EnemyBullets();
        void CollisionDetectionPlayer2Pods();
        void CollisionDetectionPlayer2PowerPods();
        void CollisionDetectionBullets2Enemies();
        bool PlayerCollide2Object(Object* object);                   // unique_ptrの値を使うため、生ポインタを使用する形で実装
        bool ObjectCollide2Object(Object *object1, Object *object2); // unique_ptrの値を使うため、生ポインタを使用する形で実装

        // その他内部関数
        void CountScore();
        void AddHighScore();
        void WriteHighScores();

        // 機能の移譲オブジェクト
        Controller _controller;
        Renderer _renderer;
        SoundPlayer _soundPlayer;        

        // ゲーム内オブジェクト格納
        Player _player;
        std::vector<std::unique_ptr<Object>> _debris{};
        std::vector<std::unique_ptr<Object>> _bullets{};
        std::vector<std::unique_ptr<Object>> _enemies{};
        std::vector<std::unique_ptr<Object>> _enemyBullets{};
        std::vector<std::unique_ptr<Object>> _explosions{};
        std::vector<std::unique_ptr<Object>> _pods{};
        std::vector<std::unique_ptr<Object>> _powerPods{};

        // レベル制御パラメータ
        int _level = 1;
        int _EnemySpawnFrame = DefaultEnemySpawnFrame;               // 敵の生成インターバル
        int _enemySpawnTimer = InitialEnemySpawnFrame;               // 敵の生成時間カウント用変数
        int _NumEnemyGenerate = DefaultNumEnemyGenerate;             // 一度に生成される敵機の数
        int _EnemySpeed = DefaultEnemySpeed;                         // 敵の接近速度
        int _EnemyFireInterval = DefaultEnemyFireInterval;           // 敵弾を生成インターバル
        int _EnemyBulletSpeed = DefaultEnemyBulletSpeed;             // 敵弾の速度
        int _EnemyHeightAmplitude = DefaultEnemyHeightAmplitude;     // 敵の横方向振幅(最初は0)
        int _podGenerateCount = DefaultPodGenerateCount;             // スコアポッドの生成カウント
        int _powerPodGenerateCount = DefaultPowerPodGenerateCount;   // パワーポッドの生成カウント

        // スコア関係
        int _score = 0;                         // 現在のスコア
        int _highScore = 0;                     // 現在のハイスコア
        std::vector<HighScore> _highscores{};   // ハイスコアテーブル
        std::string _playername = ANONYMOUS;  // ハイスコアに紐づける現在のプレイヤーネーム

        // 描画制御関係
        int _frame_count = 0; // 経過フレームのカウント
        int _bulletTimer = 0; // 弾の描画インターバル

        // ゲームの状態管理
        GameState _state;
};


#endif // GAME_H