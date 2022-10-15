#pragma once
#include "Common.hpp"
#include "Enemy.hpp"
#include "AlphaEnemy.hpp"

// ゲームシーン
class Game : public App::Scene {
   public:
    Game(const InitData& init);

    void update() override;

    void draw() const override;

   private:
    Enemy enemy;
    
    AlphaEnemy alpha_enemy;
    
    bool drawing_path=false;
};
