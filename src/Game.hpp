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
};
