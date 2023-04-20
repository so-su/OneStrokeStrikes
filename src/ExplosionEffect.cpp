#include "ExplosionEffect.hpp"

ExplosionEffect::ExplosionEffect(const Vec2& pos, double base_hue)
    :pos{pos},base_hue{base_hue}{
        for (auto& particle : particles) {
            particle = Particle{RandomVec2(Circle{30.0}), Random(-0.3, 0.1),Random(5.0, 15.0), HSV{base_hue + Random(-30.0, 30.0)}};
        }
    }

bool ExplosionEffect::update(double time) {
    // 正方形のエフェクト
    if(time < square_duration){
        // イージング関数の値
        const double easing_inv{EaseOutExpo(1.0 - time/square_duration)};
        
        Rect{Arg::center=pos.asPoint(),static_cast<int32>(30*easing_inv),static_cast<int32>(30*easing_inv)}.draw(HSV{base_hue,0.8*easing_inv}).drawFrame(0,10*easing_inv,HSV{base_hue});
        
        return true;
    }
    
    // 三角形のエフェクト
    for (const auto& particle : particles) {
        // 描画が始まってからの経過時間
        const double virtual_time{particle.start_time + time - square_duration};

        if (not InRange(virtual_time, 0.0, triangle_duration)) {
            continue;
        }

        // イージング関数の値
        const double ratio {virtual_time / triangle_duration};
        const double easing{EaseOutExpo(ratio)};
        const double easing_inv{EaseOutExpo(1.0 - ratio)};
        
        const Vec2 particle_pos{pos + particle.velocity * easing};
        
        Triangle{ particle_pos, particle.scale, particle_pos.x * 10_deg }.draw(ColorF{particle.color, easing_inv});
    }

    return time < square_duration + triangle_duration + 0.3;
}
