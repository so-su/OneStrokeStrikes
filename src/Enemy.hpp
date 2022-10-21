#pragma once
#include "Common.hpp"
#include "Polyomino.hpp"

class Enemy : public Polyomino {
   public:
    Enemy();
    
    void initialize();
    
    // ゲージを更新し、満タンになったらtrueを返す
    bool update_gauge();
    
    // 外周のゲージを描画する
    void draw_gauge()const;
    
    // 攻撃力を返す
    int32 attack_value()const;

   private:
    // 外周のパス
    Array<Line> perimeter;
    
    // 外周のゲージの長さ
    double gauge_len;
};
