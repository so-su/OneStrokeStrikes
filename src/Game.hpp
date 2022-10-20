#pragma once
#include "AlphaEnemy.hpp"
#include "Common.hpp"
#include "Enemy.hpp"
#include "ProgressBar.hpp"
#include "Player.hpp"

// ゲームシーン
class Game : public App::Scene {
   public:
    Game(const InitData& init);

    void update() override;

    void draw() const override;

   private:
    Player player;
    
    Enemy enemy;

    AlphaEnemy alpha_enemy;

    bool drawing_path = false;
};
