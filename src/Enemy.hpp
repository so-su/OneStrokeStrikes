#pragma once
#include "Common.hpp"
#include "Polyomino.hpp"

class Enemy : public Polyomino {
   public:
    Enemy(Point center);
    
    void initialize();
    
    // ゲージを更新し、満タンになったらtrueを返す
    bool update_gauge();
    
    // ゲージをリセットする
    void reset_gauge();
    
    // 外周のゲージを描画する
    void draw_gauge()const;
    
    // 攻撃力を返す
    int32 attack_value()const;
    
    void speed_up_gauge(int32 times);

   private:
    // 外周のパス
    Array<Line> perimeter;
    
    // 外周のゲージの長さ
    double gauge_len;
    
    // ゲージがたまる速さ
    double gauge_speed;
    
    // ゲージの色
    static constexpr Color gauge_color=MyColor::Orange;
};
