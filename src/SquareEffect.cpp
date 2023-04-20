#include "SquareEffect.hpp"

SquareEffect::SquareEffect(Vec2 pos, double base_hue) : pos{pos} {
    for (auto& square : squares) {
        square = Square{RandomVec2(Circle{30}), Random(-0.3, 0.1), Random(5.0, 40.0),
                   HSV{base_hue + Random(-30.0, 30.0)}};
    }
}

bool SquareEffect::update(double time) {
    for (const auto& square : squares) {
        // 描画が始まってからの経過時間
        const double virtual_time{square.start_time + time};

        if (not InRange(virtual_time, 0.0, 1.0)) {
            continue;
        }

        // イージング関数の値
        const double easing{EaseOutExpo(virtual_time)};

        const Vec2 square_pos{pos + square.offset + square.offset * 2 * time};
        const int32 square_size{static_cast<int32>(easing * square.scale)};

        Rect{Arg::center = square_pos.asPoint(), square_size, square_size}
            .rotatedAt(square_pos, square_pos.x * 6_deg)
            .draw(ColorF{square.color, 0.15})
            .drawFrame(0.5 * (1.0 - easing) * square.scale, square.color);
    }

    return time < 1.3;
}
