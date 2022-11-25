#pragma once
#include "AlphaEnemy.hpp"
#include "Common.hpp"
#include "Enemy.hpp"
#include "ProgressBar.hpp"
#include "Player.hpp"
#include "AttackShape.hpp"
#include "AttackRoulette.hpp"

// ゲームシーン
class Game : public App::Scene {
   public:
    Game(const InitData& init);

    void update() override;

    void draw() const override;

   private:
    Player player;
    
    std::array<Enemy,3> enemies;

    AlphaEnemy alpha_enemy;

    Optional<int32> drawing_path_idx;
    
    static constexpr Color background_color=Color{0,0,50};
    
    static constexpr std::array<Rect,3> enemy_frames={Rect{50,300,400,400},Rect{500,300,400,400},Rect{950,300,400,400}};
    
    // これは今のところ使わないので消してもいい
    static constexpr Rect alpha_enemy_frame=Rect{400,30,600,240};
    
    // アタックモード時にEnemyにかかるマスク
    static constexpr Rect mask=Rect{0,290,1400,440};
    
    Transition mask_alpha_transition{0.5s,0.5s};
    
    bool attack_mode;
    
    Stopwatch attack_mode_timer;
    
    std::array<double,3> respawn_timers;
    
    double respawn_time;
    
    std::array<double,3> vanishing_timers;
    
    enum class AllClearStatus{
        EnemyAliveExists,
        LastIsVanishing,
        LastHasVanished,
    };
    
    AllClearStatus all_clear_status;
    
    double roulette_duration;
    
    AttackRoulette roulette;
    
    bool pause;
    
    AttackShape* attack_shape;
};
