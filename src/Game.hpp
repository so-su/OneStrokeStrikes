#pragma once
#include "AlphaEnemy.hpp"
#include "Common.hpp"
#include "Enemy.hpp"
#include "ProgressBar.hpp"
#include "Player.hpp"
#include <queue>

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
    
    std::queue<int32> enemy_idx_queue;

    Optional<int32> drawing_path_idx;
    
    static constexpr std::array<Rect,3> enemy_frames={Rect{50,300,400,400},Rect{500,300,400,400},Rect{950,300,400,400}};
    
    static constexpr Rect alpha_enemy_frame=Rect{400,30,600,240};
    
    static constexpr Rect mask=Rect{0,290,1400,440};
    
    Transition mask_alpha_transition{0.5s,0.5s};
    
    bool attack_mode;
    
    Stopwatch attack_mode_timer;
};
