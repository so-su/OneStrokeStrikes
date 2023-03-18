#pragma once
#include"Common.hpp"

struct SquareEffect : IEffect {
public:
    SquareEffect(Vec2 pos, double base_hue);

    bool update(double time) override;
    
private:
    struct Square {
        Vec2 offset; // 中心からのオフセット
        double start_time; // 描画が始まる時間
        double scale; // 正方形の一辺の長さ
        ColorF color; // 色
    };

    // エフェクトの中心座標
    Vec2 pos;
    
    // 描画する正方形の個数
    static constexpr int32 num_square{8};

    // 描画する正方形の配列
    std::array<Square, num_square> squares;
};
