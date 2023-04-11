#pragma once
#include "AlphaEnemy.hpp"
#include "AttackRoulette.hpp"
#include "AttackShape.hpp"
#include "Common.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "ProgressBar.hpp"
#include "StringEffect.hpp"

// ゲームシーン
class Game : public App::Scene {
   public:
    Game(const InitData& init);

    void update() override;

    void draw() const override;

   private:
    // 図形で攻撃する位置を選んでいるときの処理
    void shape_attack_update();

    // 攻撃モード時の処理
    void attack_mode_update();

    // 攻撃モードから抜ける処理
    void get_out_of_attack_mode();

    // Enemyたちの消滅を進める
    void update_to_vanish_enemies();

    // SPを消費して敵を一掃する
    void use_sp();

    // 一筆書きのパスを更新し、一筆書きが成功したならEnemyを消滅させる準備をする
    void update_one_stroke_path();

    // APを消費して攻撃モードに遷移する
    void use_ap();
    
    // APバーとSPバーにぼかし処理を施す
    void blur_bars() const;

    // プレイヤーのステータスを管理
    Player player;

    // 下段のポリオミノの敵たちの配列
    std::array<Enemy, 3> enemies{Enemy(Point{250, 500}, getData().difficulty),
                                 Enemy(Point{700, 500}, getData().difficulty),
                                 Enemy(Point{1150, 500}, getData().difficulty)};

    // 上段のポリオミノの敵
    AlphaEnemy alpha_enemy{getData().difficulty};

    // 一筆書き中のEnemyのインデックス
    Optional<int32> drawing_path_idx{none};

    // maskの透過率を滑らかに変化させる
    Transition mask_alpha_transition{0.5s, 0.5s};

    // 攻撃モードかどうか
    bool attack_mode{false};

    // 攻撃モードに入ってからの時間を管理
    Stopwatch attack_mode_timer;

    // respawn_timeになるまでの時間を管理
    std::array<double, 3> respawn_timers{0.0, 0.0, 0.0};
    
    // スポーンしてからの時間を管理
    std::array<double, 3> times_since_spawn{0.0, 0.0, 0.0};

    // Enemyが消滅し始めてからの時間を管理
    std::array<double, 3> vanishing_timers;

    // All Clearの状態になってからすべてのEnemyが消滅するまで待つために導入した状態
    enum class AllClearStatus {
        EnemyAliveExists,
        LastIsVanishing,
        LastHasVanished,
    };

    AllClearStatus all_clear_status{AllClearStatus::EnemyAliveExists};

    AttackRoulette roulette;

    // ルーレットが回り始めてから止まるまでの時間
    double roulette_duration;

    // 一時停止の状態かどうか
    bool pause{false};

    AttackShape* attack_shape{nullptr};

    // 3つのEnemyを囲う枠
    static constexpr std::array<Rect, 3> enemy_frames{Rect{50, 300, 400, 400},
                                                      Rect{500, 300, 400, 400},
                                                      Rect{950, 300, 400, 400}};

    // アタックモード時にEnemyにかかるマスク
    static constexpr Rect mask{0, 290, 1400, 440};
    
    // スクリーンのサイズ
    static constexpr Size sceneSize{ 1400, 800 };
    
    // APバーのぼかし処理のため
    RenderTexture gaussianA1_ap{ sceneSize };
    RenderTexture gaussianA16_ap{ sceneSize / 16 }, gaussianB16_ap{ sceneSize / 16 };
    
    // SPバーのぼかし処理のため
    RenderTexture gaussianA1_sp{ sceneSize };
    RenderTexture gaussianA16_sp{ sceneSize / 16 }, gaussianB16_sp{ sceneSize / 16 };
    
    // StringEffectの描画を管理する
    Effect effect;
    
    // 初めのupdateが呼ばれてからの経過時間
    double time_since_first_update{0.0};
    
    // スタートまでのカウントダウン
    int32 countdown{3};
};
