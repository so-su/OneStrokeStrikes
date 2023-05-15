#pragma once
#include "Common.hpp"

struct ExplosionEffect : IEffect {
public:
    ExplosionEffect(const Vec2& start, double base_hue);

    bool update(double time) override;
    
private:
    struct Particle {
        Vec2 velocity; // 拡散する速さ
        double start_time; // 描画が始まる時間
        double scale; // 正方形の一辺の長さ
        ColorF color; // 色
    };
    
    // エフェクトの中心座標
    const Vec2 pos;
    
    // エフェクトのベースとなる色相
    const double base_hue;
    
    // 破片の個数
    static constexpr int32 num_particle{30};
    
    // 破片の配列
    std::array<Particle,num_particle> particles;
    
    // 正方形のエフェクトが出ている時間
    static constexpr double square_duration{0.3};
    
    // 三角形のエフェクトが出ている時間
    static constexpr double triangle_duration{0.8};
};
